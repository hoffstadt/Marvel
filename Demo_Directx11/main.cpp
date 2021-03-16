#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define UNICODE
#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <assert.h>
#include <wrl.h>

//-----------------------------------------------------------------------------
// ComPtr Wrapper
//-----------------------------------------------------------------------------
template <typename T>
using mvComPtr = Microsoft::WRL::ComPtr<T>;

//-----------------------------------------------------------------------------
// Variables
//-----------------------------------------------------------------------------
static HWND                             g_hwnd;
static mvComPtr<ID3D11Device>           g_device;
static mvComPtr<ID3D11DeviceContext>    g_context;
static mvComPtr<IDXGISwapChain>         g_swapChain;
static mvComPtr<ID3D11Texture2D>        g_frameBuffer;
static mvComPtr<ID3D11RenderTargetView> g_frameBufferView;
static mvComPtr<ID3DBlob>               g_vsBlob;
static mvComPtr<ID3D11VertexShader>     g_vertexShader;
static mvComPtr<ID3D11PixelShader>      g_pixelShader;
static mvComPtr<ID3D11InputLayout>      g_inputLayout;
static mvComPtr<ID3D11Buffer>           g_vertexBuffer;
static mvComPtr<ID3D11Buffer>           g_indexBuffer;
static UINT                             g_numVerts;
static UINT                             g_stride;
static UINT                             g_offset;
static const int                        g_width = 1024;
static const int                        g_height = 768;

//-----------------------------------------------------------------------------
// Vertex Data and Index Data
//-----------------------------------------------------------------------------
static const float g_vertexData[] = { // x, y, r, g, b, a
     0.0f,  0.5f, 1.f, 1.f, 1.f, 1.f,
     0.5f, -0.5f, 1.f, 1.f, 1.f, 1.f,
    -0.5f, -0.5f, 1.f, 1.f, 1.f, 1.f
};

static const unsigned short g_indices[3] = { 0, 1, 2 };

//-----------------------------------------------------------------------------
// Windows Procedure
//-----------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

int main(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int /*nShowCmd*/)
{

    //-----------------------------------------------------------------------------
    // Create and Open Window
    //-----------------------------------------------------------------------------
    {
        WNDCLASSEXW winClass = {};
        winClass.cbSize = sizeof(WNDCLASSEXW);
        winClass.style = CS_HREDRAW | CS_VREDRAW;
        winClass.lpfnWndProc = &WndProc;
        winClass.hInstance = hInstance;
        winClass.hIcon = LoadIconW(0, IDI_APPLICATION);
        winClass.hCursor = LoadCursorW(0, IDC_ARROW);
        winClass.lpszClassName = L"MyWindowClass";
        winClass.hIconSm = LoadIconW(0, IDI_APPLICATION);

        if (!RegisterClassExW(&winClass)) {
            MessageBoxA(0, "RegisterClassEx failed", "Fatal Error", MB_OK);
            return GetLastError();
        }

        RECT initialRect = { 0, 0, g_width, g_height };
        AdjustWindowRectEx(&initialRect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_OVERLAPPEDWINDOW);
        LONG initialWidth = initialRect.right - initialRect.left;
        LONG initialHeight = initialRect.bottom - initialRect.top;

        g_hwnd = CreateWindowExW(WS_EX_OVERLAPPEDWINDOW,
            winClass.lpszClassName,
            L"Hello Triangle - Directx11",
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            CW_USEDEFAULT, CW_USEDEFAULT,
            initialWidth,
            initialHeight,
            0, 0, hInstance, 0);

        if (!g_hwnd) {
            MessageBoxA(0, "CreateWindowEx failed", "Fatal Error", MB_OK);
            return GetLastError();
        }
    }

    //-----------------------------------------------------------------------------
    // Create D3D11 Device and Context and Swap Chain
    //-----------------------------------------------------------------------------
    {
        DXGI_SWAP_CHAIN_DESC sd = {};
        sd.BufferDesc.Width = g_width;
        sd.BufferDesc.Height = g_height;
        sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 0;
        sd.BufferDesc.RefreshRate.Denominator = 0;
        sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.BufferCount = 2;
        sd.OutputWindow = g_hwnd;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; //DXGI_SWAP_EFFECT_DISCARD;
        sd.Flags = 0;

        // create device and front/back buffers, and swap chain and rendering context
        HRESULT hResult = D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            D3D11_CREATE_DEVICE_DEBUG,
            nullptr,
            0,
            D3D11_SDK_VERSION,
            &sd,
            g_swapChain.GetAddressOf(),
            g_device.GetAddressOf(),
            nullptr,
            g_context.GetAddressOf()
        );
        assert(SUCCEEDED(hResult));
    }

    //-----------------------------------------------------------------------------
    // Create Framebuffer Render Target
    //-----------------------------------------------------------------------------
    {
        HRESULT hResult = g_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)g_frameBuffer.GetAddressOf());
        assert(SUCCEEDED(hResult));

        hResult = g_device->CreateRenderTargetView(g_frameBuffer.Get(), 0, g_frameBufferView.GetAddressOf());
        assert(SUCCEEDED(hResult));
    }

    //-----------------------------------------------------------------------------
    // Create Vertex Shader
    //-----------------------------------------------------------------------------
    {
        mvComPtr<ID3DBlob> shaderCompileErrorsBlob;
        HRESULT hResult = D3DCompileFromFile(L"../../Demo_Directx11/shaders/shaders.hlsl", 
            nullptr, nullptr, "vs_main", "vs_5_0", 0, 0, g_vsBlob.GetAddressOf(), shaderCompileErrorsBlob.GetAddressOf());
        if (FAILED(hResult))
        {
            const char* errorString = nullptr;
            if (hResult == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
                errorString = "Could not compile shader; file not found";
            else if (shaderCompileErrorsBlob)
                errorString = (const char*)shaderCompileErrorsBlob->GetBufferPointer();
            MessageBoxA(0, errorString, "Shader Compiler Error", MB_ICONERROR | MB_OK);
            return 1;
        }

        hResult = g_device->CreateVertexShader(g_vsBlob->GetBufferPointer(), 
            g_vsBlob->GetBufferSize(), nullptr, g_vertexShader.GetAddressOf());
        assert(SUCCEEDED(hResult));
    }

    //-----------------------------------------------------------------------------
    // Create Pixel Shader
    //-----------------------------------------------------------------------------
    {
        mvComPtr<ID3DBlob> psBlob;
        mvComPtr<ID3DBlob> shaderCompileErrorsBlob;
        HRESULT hResult = D3DCompileFromFile(L"../../Demo_Directx11/shaders/shaders.hlsl", 
            nullptr, nullptr, "ps_main", "ps_5_0", 0, 0, psBlob.GetAddressOf(), shaderCompileErrorsBlob.GetAddressOf());
        if (FAILED(hResult))
        {
            const char* errorString = nullptr;
            if (hResult == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
                errorString = "Could not compile shader; file not found";
            else if (shaderCompileErrorsBlob)
                errorString = (const char*)shaderCompileErrorsBlob->GetBufferPointer();

            MessageBoxA(0, errorString, "Shader Compiler Error", MB_ICONERROR | MB_OK);
            return 1;
        }

        hResult = g_device->CreatePixelShader(psBlob->GetBufferPointer(), 
            psBlob->GetBufferSize(), nullptr, g_pixelShader.GetAddressOf());
        assert(SUCCEEDED(hResult));
    }

    //-----------------------------------------------------------------------------
    // Create Input Layout
    //-----------------------------------------------------------------------------
    {
        D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
        {
            { "Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "Color", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };

        HRESULT hResult = g_device->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), 
            g_vsBlob->GetBufferPointer(), g_vsBlob->GetBufferSize(), g_inputLayout.GetAddressOf());
        assert(SUCCEEDED(hResult));
    }

    //-----------------------------------------------------------------------------
    // Create Vertex Buffer
    //-----------------------------------------------------------------------------
    {
        g_stride = 6 * sizeof(float);
        g_numVerts = sizeof(g_vertexData) / g_stride;
        g_offset = 0;

        D3D11_BUFFER_DESC vertexBufferDesc = {};
        vertexBufferDesc.ByteWidth = sizeof(g_vertexData);
        vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        D3D11_SUBRESOURCE_DATA vertexSubresourceData = { g_vertexData };

        HRESULT hResult = g_device->CreateBuffer(&vertexBufferDesc, 
            &vertexSubresourceData, g_vertexBuffer.GetAddressOf());
        assert(SUCCEEDED(hResult));
    }

    //-----------------------------------------------------------------------------
    // Create Index Buffer
    //-----------------------------------------------------------------------------
    {
        // Fill in a buffer description.
        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.ByteWidth = UINT(sizeof(unsigned short) * 3);
        bufferDesc.StructureByteStride = sizeof(unsigned short);
        bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bufferDesc.CPUAccessFlags = 0u;
        bufferDesc.MiscFlags = 0u;

        // Define the resource data.
        D3D11_SUBRESOURCE_DATA InitData = {};
        InitData.pSysMem = g_indices;

        // Create the buffer with the device.
        g_device->CreateBuffer(&bufferDesc, &InitData, g_indexBuffer.GetAddressOf());
    }

    //-----------------------------------------------------------------------------
    // Main loop
    //-----------------------------------------------------------------------------
    bool isRunning = true;
    while (isRunning)
    {
        // poll for and process events
        MSG msg = {};
        while (PeekMessageW(&msg, 0, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                isRunning = false;
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }

        // clear render target
        float backgroundColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        g_context->ClearRenderTargetView(g_frameBufferView.Get(), backgroundColor);

        // set viewport
        RECT winRect;
        GetClientRect(g_hwnd, &winRect);
        D3D11_VIEWPORT viewport = { 0.0f, 0.0f, (FLOAT)(winRect.right - winRect.left), (FLOAT)(winRect.bottom - winRect.top), 0.0f, 1.0f };
        g_context->RSSetViewports(1, &viewport);

        // bind frame buffer
        g_context->OMSetRenderTargets(1, g_frameBufferView.GetAddressOf(), nullptr);

        // bind primitive topology
        g_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        // bind vertex layout
        g_context->IASetInputLayout(g_inputLayout.Get());

        // bind shaders
        g_context->VSSetShader(g_vertexShader.Get(), nullptr, 0);
        g_context->PSSetShader(g_pixelShader.Get(), nullptr, 0);

        // bind index and vertex buffers
        g_context->IASetIndexBuffer(g_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
        g_context->IASetVertexBuffers(0, 1, g_vertexBuffer.GetAddressOf(), &g_stride, &g_offset);

        // draw
        g_context->DrawIndexed(3, 0u, 0u);

        // swap front and back buffers
        g_swapChain->Present(1, 0);
    }

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    LRESULT result = 0;
    switch (msg)
    {
    case WM_KEYDOWN:
    {
        if (wparam == VK_ESCAPE)
            DestroyWindow(hwnd);
        break;
    }
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        break;
    }
    default:
        result = DefWindowProcW(hwnd, msg, wparam, lparam);
    }
    return result;
}