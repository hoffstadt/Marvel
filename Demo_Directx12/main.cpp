#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define UNICODE
#include <windows.h>

#define GLM_FORCE_SSE42 1
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES 1
#define GLM_FORCE_LEFT_HANDED
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
using namespace glm;

#include <assert.h>
#include "d3d12.h"
#include <D3Dcompiler.h>
#include <dxgi1_4.h>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>
#include <direct.h>

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

inline void ThrowIfFailed(HRESULT hr)
{
    if (FAILED(hr))
    {
        throw std::exception();
    }
}

struct Vertex
{
    float position[3];
    float color[3];
};

// Uniform data
struct
{
    glm::mat4 projectionMatrix;
    glm::mat4 modelMatrix;
    glm::mat4 viewMatrix;
} uboVS;

int main(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int /*nShowCmd*/)
{
    // Open a window
    HWND hwnd;
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

        RECT initialRect = { 0, 0, 1024, 768 };
        AdjustWindowRectEx(&initialRect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_OVERLAPPEDWINDOW);
        LONG initialWidth = initialRect.right - initialRect.left;
        LONG initialHeight = initialRect.bottom - initialRect.top;

        hwnd = CreateWindowExW(WS_EX_OVERLAPPEDWINDOW,
            winClass.lpszClassName,
            L"Hello Triangle - Directx12",
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            CW_USEDEFAULT, CW_USEDEFAULT,
            initialWidth,
            initialHeight,
            0, 0, hInstance, 0);

        if (!hwnd) {
            MessageBoxA(0, "CreateWindowEx failed", "Fatal Error", MB_OK);
            return GetLastError();
        }
    }

    unsigned mWidth, mHeight;

    Vertex mVertexBufferData[3] = {
        {{1.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{-1.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}} };

    uint32_t mIndexBufferData[3] = { 0, 1, 2 };

    std::chrono::time_point<std::chrono::steady_clock> tStart, tEnd;
    float mElapsedTime = 0.0f;

    static const UINT backbufferCount = 2;

    // Initialization
    IDXGIFactory4* mFactory = nullptr;
    IDXGIAdapter1* mAdapter = nullptr;
#if defined(_DEBUG)
    ID3D12Debug1* mDebugController = nullptr;
    ID3D12DebugDevice* mDebugDevice;
#endif
    ID3D12Device* mDevice = nullptr;
    ID3D12CommandQueue* mCommandQueue = nullptr;
    ID3D12CommandAllocator* mCommandAllocator = nullptr;
    ID3D12GraphicsCommandList* mCommandList = nullptr;

    // Current Frame
    UINT mCurrentBuffer;
    ID3D12DescriptorHeap* mRtvHeap = nullptr;
    ID3D12Resource* mRenderTargets[backbufferCount];
    for (size_t i = 0; i < backbufferCount; ++i)
        mRenderTargets[i] = nullptr;
    IDXGISwapChain3* mSwapchain = nullptr;

    // Resources
    D3D12_VIEWPORT mViewport;
    D3D12_RECT mSurfaceSize;

    ID3D12Resource* mVertexBuffer = nullptr;
    ID3D12Resource* mIndexBuffer = nullptr;

    ID3D12Resource* mUniformBuffer = nullptr;
    ID3D12DescriptorHeap* mUniformBufferHeap = nullptr;
    UINT8* mMappedUniformBuffer = nullptr;

    D3D12_VERTEX_BUFFER_VIEW mVertexBufferView;
    D3D12_INDEX_BUFFER_VIEW mIndexBufferView;

    UINT mRtvDescriptorSize;
    ID3D12RootSignature* mRootSignature;
    ID3D12PipelineState* mPipelineState;

    // Sync
    UINT mFrameIndex;
    HANDLE mFenceEvent;
    ID3D12Fence* mFence = nullptr;
    UINT64 mFenceValue = 0;

    //-----------------------------------------------------------------------------
    // Initialize API
    //-----------------------------------------------------------------------------

    // Create Factory

    UINT dxgiFactoryFlags = 0;
#if defined(_DEBUG)
    ID3D12Debug* debugController;
    ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
    ThrowIfFailed(
        debugController->QueryInterface(IID_PPV_ARGS(&mDebugController)));
    mDebugController->EnableDebugLayer();
    mDebugController->SetEnableGPUBasedValidation(true);

    dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;

    debugController->Release();
    debugController = nullptr;

#endif
    ThrowIfFailed(
        CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&mFactory)));

    // Create Adapter

    for (UINT adapterIndex = 0;
        DXGI_ERROR_NOT_FOUND !=
        mFactory->EnumAdapters1(adapterIndex, &mAdapter);
        ++adapterIndex)
    {
        DXGI_ADAPTER_DESC1 desc;
        mAdapter->GetDesc1(&desc);

        if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
        {
            // Don't select the Basic Render Driver adapter.
            continue;
        }

        // Check to see if the adapter supports Direct3D 12, but don't create
        // the actual device yet.
        if (SUCCEEDED(D3D12CreateDevice(mAdapter, D3D_FEATURE_LEVEL_12_0,
            _uuidof(ID3D12Device), nullptr)))
        {
            break;
        }

        // We won't use this adapter, so release it
        mAdapter->Release();
    }

    // Create Device
    ID3D12Device* pDev = nullptr;
    ThrowIfFailed(D3D12CreateDevice(mAdapter, D3D_FEATURE_LEVEL_12_0,
        IID_PPV_ARGS(&mDevice)));

    mDevice->SetName(L"Hello Triangle Device");

#if defined(_DEBUG)
    // Get debug device
    ThrowIfFailed(mDevice->QueryInterface(&mDebugDevice));
#endif

    // Create Command Queue
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    ThrowIfFailed(
        mDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&mCommandQueue)));

    // Create Command Allocator
    ThrowIfFailed(mDevice->CreateCommandAllocator(
        D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mCommandAllocator)));

    // Sync
    ThrowIfFailed(
        mDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence)));

    mWidth = clamp(1024u, 1u, 0xffffu);
    mHeight = clamp(768u, 1u, 0xffffu);

    // WAITING FOR THE FRAME TO COMPLETE BEFORE CONTINUING IS NOT BEST PRACTICE.
    // This is code implemented as such for simplicity. The
    // D3D12HelloFrameBuffering sample illustrates how to use fences for
    // efficient resource usage and to maximize GPU utilization.

    // Signal and increment the fence value.
    const UINT64 fence = mFenceValue;
    ThrowIfFailed(mCommandQueue->Signal(mFence, fence));
    mFenceValue++;

    // Wait until the previous frame is finished.
    if (mFence->GetCompletedValue() < fence)
    {
        ThrowIfFailed(mFence->SetEventOnCompletion(fence, mFenceEvent));
        WaitForSingleObjectEx(mFenceEvent, INFINITE, false);
    }

    // destroyFrameBuffer
    for (size_t i = 0; i < backbufferCount; ++i)
    {
        if (mRenderTargets[i])
        {
            mRenderTargets[i]->Release();
            mRenderTargets[i] = 0;
        }
    }
    if (mRtvHeap)
    {
        mRtvHeap->Release();
        mRtvHeap = nullptr;
    }

    // setupSwapchain
    mSurfaceSize.left = 0;
    mSurfaceSize.top = 0;
    mSurfaceSize.right = static_cast<LONG>(mWidth);
    mSurfaceSize.bottom = static_cast<LONG>(mHeight);

    mViewport.TopLeftX = 0.0f;
    mViewport.TopLeftY = 0.0f;
    mViewport.Width = static_cast<float>(mWidth);
    mViewport.Height = static_cast<float>(mHeight);
    mViewport.MinDepth = .1f;
    mViewport.MaxDepth = 1000.f;

    // Update Uniforms
    float zoom = 2.5f;

    // Update matrices
    uboVS.projectionMatrix =
        glm::perspective(45.0f, (float)mWidth / (float)mHeight, 0.01f, 1024.0f);

    uboVS.viewMatrix =
        glm::translate(glm::identity<mat4>(), vec3(0.0f, 0.0f, zoom));

    uboVS.modelMatrix = glm::identity<mat4>();

    if (mSwapchain != nullptr)
    {
        mSwapchain->ResizeBuffers(backbufferCount, mWidth, mHeight,
            DXGI_FORMAT_R8G8B8A8_UNORM, 0);
    }
    else
    {
        DXGI_SWAP_CHAIN_DESC1 swapchainDesc = {};
        swapchainDesc.BufferCount = backbufferCount;
        swapchainDesc.Width = 1024;
        swapchainDesc.Height = 768;
        swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swapchainDesc.SampleDesc.Count = 1;

        IDXGISwapChain1* swapchain;
        HRESULT hr = mFactory->CreateSwapChainForHwnd(
            mCommandQueue, hwnd, &swapchainDesc, nullptr, nullptr, &swapchain);

        HRESULT swapchainSupport = swapchain->QueryInterface(
            __uuidof(IDXGISwapChain3), (void**)&swapchain);
        if (SUCCEEDED(swapchainSupport))
        {
            mSwapchain = (IDXGISwapChain3*)swapchain;
        }
    }
    mFrameIndex = mSwapchain->GetCurrentBackBufferIndex();

    // initFrameBuffer
    mCurrentBuffer = mSwapchain->GetCurrentBackBufferIndex();

    // Create descriptor heaps.
    {
        // Describe and create a render target view (RTV) descriptor heap.
        D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
        rtvHeapDesc.NumDescriptors = backbufferCount;
        rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        ThrowIfFailed(mDevice->CreateDescriptorHeap(&rtvHeapDesc,
            IID_PPV_ARGS(&mRtvHeap)));

        mRtvDescriptorSize = mDevice->GetDescriptorHandleIncrementSize(
            D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    }

    // Create frame resources.
    {
        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle(
            mRtvHeap->GetCPUDescriptorHandleForHeapStart());

        // Create a RTV for each frame.
        for (UINT n = 0; n < backbufferCount; n++)
        {
            ThrowIfFailed(
                mSwapchain->GetBuffer(n, IID_PPV_ARGS(&mRenderTargets[n])));
            mDevice->CreateRenderTargetView(mRenderTargets[n], nullptr,
                rtvHandle);
            rtvHandle.ptr += (1 * mRtvDescriptorSize);
        }
    }

    //-----------------------------------------------------------------------------
    // Initialize Resources
    //-----------------------------------------------------------------------------
    // Create the root signature.
    {
        D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData = {};

        // This is the highest version the sample supports. If
        // CheckFeatureSupport succeeds, the HighestVersion returned will not be
        // greater than this.
        featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;

        if (FAILED(mDevice->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE,
            &featureData,
            sizeof(featureData))))
        {
            featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
        }

        D3D12_DESCRIPTOR_RANGE1 ranges[1];
        ranges[0].BaseShaderRegister = 0;
        ranges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
        ranges[0].NumDescriptors = 1;
        ranges[0].RegisterSpace = 0;
        ranges[0].OffsetInDescriptorsFromTableStart = 0;
        ranges[0].Flags = D3D12_DESCRIPTOR_RANGE_FLAG_NONE;

        D3D12_ROOT_PARAMETER1 rootParameters[1];
        rootParameters[0].ParameterType =
            D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

        rootParameters[0].DescriptorTable.NumDescriptorRanges = 1;
        rootParameters[0].DescriptorTable.pDescriptorRanges = ranges;

        D3D12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
        rootSignatureDesc.Version = D3D_ROOT_SIGNATURE_VERSION_1_1;
        rootSignatureDesc.Desc_1_1.Flags =
            D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
        rootSignatureDesc.Desc_1_1.NumParameters = 1;
        rootSignatureDesc.Desc_1_1.pParameters = rootParameters;
        rootSignatureDesc.Desc_1_1.NumStaticSamplers = 0;
        rootSignatureDesc.Desc_1_1.pStaticSamplers = nullptr;

        ID3DBlob* signature;
        ID3DBlob* error;
        try
        {
            ThrowIfFailed(D3D12SerializeVersionedRootSignature(
                &rootSignatureDesc, &signature, &error));
            ThrowIfFailed(mDevice->CreateRootSignature(
                0, signature->GetBufferPointer(), signature->GetBufferSize(),
                IID_PPV_ARGS(&mRootSignature)));
            mRootSignature->SetName(L"Hello Triangle Root Signature");
        }
        catch (std::exception e)
        {
            const char* errStr = (const char*)error->GetBufferPointer();
            std::cout << errStr;
            error->Release();
            error = nullptr;
        }

        if (signature)
        {
            signature->Release();
            signature = nullptr;
        }
    }

    // Create the pipeline state, which includes compiling and loading shaders.
    {
        ID3DBlob* vertexShader = nullptr;
        ID3DBlob* pixelShader = nullptr;
        ID3DBlob* errors = nullptr;

#if defined(_DEBUG)
        // Enable better shader debugging with the graphics debugging tools.
        UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
        UINT compileFlags = 0;
#endif

        std::string path = "";
        char pBuf[1024];

        _getcwd(pBuf, 1024);
        path = pBuf;
        path += "\\";
        std::wstring wpath = std::wstring(path.begin(), path.end());

        std::string vertCompiledPath = path, fragCompiledPath = path;
        vertCompiledPath += "..\\..\\Demo_Directx12\\shaders\\triangle.vert.dxbc";
        fragCompiledPath += "..\\..\\Demo_Directx12\\shaders\\triangle.frag.dxbc";

#define COMPILESHADERS
#ifdef COMPILESHADERS
        std::wstring vertPath =
            wpath + L"..\\..\\Demo_Directx12\\shaders\\triangle.vert.hlsl";
        std::wstring fragPath =
            wpath + L"..\\..\\Demo_Directx12\\shaders\\triangle.frag.hlsl";

        try
        {
            ThrowIfFailed(D3DCompileFromFile(vertPath.c_str(), nullptr, nullptr,
                "main", "vs_5_0", compileFlags, 0,
                &vertexShader, &errors));
            ThrowIfFailed(D3DCompileFromFile(fragPath.c_str(), nullptr, nullptr,
                "main", "ps_5_0", compileFlags, 0,
                &pixelShader, &errors));
        }
        catch (std::exception e)
        {
            const char* errStr = (const char*)errors->GetBufferPointer();
            std::cout << errStr;
            errors->Release();
            errors = nullptr;
        }

        std::ofstream vsOut(vertCompiledPath, std::ios::out | std::ios::binary),
            fsOut(fragCompiledPath, std::ios::out | std::ios::binary);

        vsOut.write((const char*)vertexShader->GetBufferPointer(),
            vertexShader->GetBufferSize());
        fsOut.write((const char*)pixelShader->GetBufferPointer(),
            pixelShader->GetBufferSize());

#else
        std::vector<char> vsBytecodeData = readFile(vertCompiledPath);
        std::vector<char> fsBytecodeData = readFile(fragCompiledPath);

#endif
        // Define the vertex input layout.
        D3D12_INPUT_ELEMENT_DESC inputElementDescs[] = {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
             D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
            {"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
             D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0} };

        // Create the UBO.
        {
            // Note: using upload heaps to transfer static data like vert
            // buffers is not recommended. Every time the GPU needs it, the
            // upload heap will be marshalled over. Please read up on Default
            // Heap usage. An upload heap is used here for code simplicity and
            // because there are very few verts to actually transfer.
            D3D12_HEAP_PROPERTIES heapProps;
            heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
            heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
            heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
            heapProps.CreationNodeMask = 1;
            heapProps.VisibleNodeMask = 1;

            D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
            heapDesc.NumDescriptors = 1;
            heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
            heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
            ThrowIfFailed(mDevice->CreateDescriptorHeap(
                &heapDesc, IID_PPV_ARGS(&mUniformBufferHeap)));

            D3D12_RESOURCE_DESC uboResourceDesc;
            uboResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
            uboResourceDesc.Alignment = 0;
            uboResourceDesc.Width = (sizeof(uboVS) + 255) & ~255;
            uboResourceDesc.Height = 1;
            uboResourceDesc.DepthOrArraySize = 1;
            uboResourceDesc.MipLevels = 1;
            uboResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
            uboResourceDesc.SampleDesc.Count = 1;
            uboResourceDesc.SampleDesc.Quality = 0;
            uboResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
            uboResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

            ThrowIfFailed(mDevice->CreateCommittedResource(
                &heapProps, D3D12_HEAP_FLAG_NONE, &uboResourceDesc,
                D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
                IID_PPV_ARGS(&mUniformBuffer)));
            mUniformBufferHeap->SetName(
                L"Constant Buffer Upload Resource Heap");

            D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
            cbvDesc.BufferLocation = mUniformBuffer->GetGPUVirtualAddress();
            cbvDesc.SizeInBytes =
                (sizeof(uboVS) + 255) &
                ~255; // CB size is required to be 256-byte aligned.

            D3D12_CPU_DESCRIPTOR_HANDLE cbvHandle(
                mUniformBufferHeap->GetCPUDescriptorHandleForHeapStart());
            cbvHandle.ptr =
                cbvHandle.ptr + mDevice->GetDescriptorHandleIncrementSize(
                    D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) *
                0;

            mDevice->CreateConstantBufferView(&cbvDesc, cbvHandle);

            // We do not intend to read from this resource on the CPU. (End is
            // less than or equal to begin)
            D3D12_RANGE readRange;
            readRange.Begin = 0;
            readRange.End = 0;

            ThrowIfFailed(mUniformBuffer->Map(
                0, &readRange,
                reinterpret_cast<void**>(&mMappedUniformBuffer)));
            memcpy(mMappedUniformBuffer, &uboVS, sizeof(uboVS));
            mUniformBuffer->Unmap(0, &readRange);
        }

        // Describe and create the graphics pipeline state object (PSO).
        D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
        psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
        psoDesc.pRootSignature = mRootSignature;

        D3D12_SHADER_BYTECODE vsBytecode;
        D3D12_SHADER_BYTECODE psBytecode;

#ifdef COMPILESHADERS
        vsBytecode.pShaderBytecode = vertexShader->GetBufferPointer();
        vsBytecode.BytecodeLength = vertexShader->GetBufferSize();

        psBytecode.pShaderBytecode = pixelShader->GetBufferPointer();
        psBytecode.BytecodeLength = pixelShader->GetBufferSize();
#else
        vsBytecode.pShaderBytecode = vsBytecodeData.data();
        vsBytecode.BytecodeLength = vsBytecodeData.size();

        psBytecode.pShaderBytecode = fsBytecodeData.data();
        psBytecode.BytecodeLength = fsBytecodeData.size();
#endif

        psoDesc.VS = vsBytecode;
        psoDesc.PS = psBytecode;

        D3D12_RASTERIZER_DESC rasterDesc;
        rasterDesc.FillMode = D3D12_FILL_MODE_SOLID;
        rasterDesc.CullMode = D3D12_CULL_MODE_NONE;
        rasterDesc.FrontCounterClockwise = FALSE;
        rasterDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
        rasterDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
        rasterDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
        rasterDesc.DepthClipEnable = TRUE;
        rasterDesc.MultisampleEnable = FALSE;
        rasterDesc.AntialiasedLineEnable = FALSE;
        rasterDesc.ForcedSampleCount = 0;
        rasterDesc.ConservativeRaster =
            D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

        psoDesc.RasterizerState = rasterDesc;

        D3D12_BLEND_DESC blendDesc;
        blendDesc.AlphaToCoverageEnable = FALSE;
        blendDesc.IndependentBlendEnable = FALSE;
        const D3D12_RENDER_TARGET_BLEND_DESC defaultRenderTargetBlendDesc = {
            FALSE,
            FALSE,
            D3D12_BLEND_ONE,
            D3D12_BLEND_ZERO,
            D3D12_BLEND_OP_ADD,
            D3D12_BLEND_ONE,
            D3D12_BLEND_ZERO,
            D3D12_BLEND_OP_ADD,
            D3D12_LOGIC_OP_NOOP,
            D3D12_COLOR_WRITE_ENABLE_ALL,
        };
        for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
            blendDesc.RenderTarget[i] = defaultRenderTargetBlendDesc;

        psoDesc.BlendState = blendDesc;
        psoDesc.DepthStencilState.DepthEnable = FALSE;
        psoDesc.DepthStencilState.StencilEnable = FALSE;
        psoDesc.SampleMask = UINT_MAX;
        psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        psoDesc.NumRenderTargets = 1;
        psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
        psoDesc.SampleDesc.Count = 1;
        try
        {
            ThrowIfFailed(mDevice->CreateGraphicsPipelineState(
                &psoDesc, IID_PPV_ARGS(&mPipelineState)));
        }
        catch (std::exception e)
        {
            std::cout << "Failed to create Graphics Pipeline!";
        }

        if (vertexShader)
        {
            vertexShader->Release();
            vertexShader = nullptr;
        }

        if (pixelShader)
        {
            pixelShader->Release();
            pixelShader = nullptr;
        }
    }

    // Create the command list.
    ThrowIfFailed(mDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
        mCommandAllocator, mPipelineState,
        IID_PPV_ARGS(&mCommandList)));
    mCommandList->SetName(L"Hello Triangle Command List");

    // Command lists are created in the recording state, but there is nothing
    // to record yet. The main loop expects it to be closed, so close it now.
    ThrowIfFailed(mCommandList->Close());

    // Create the vertex buffer.
    {
        const UINT vertexBufferSize = sizeof(mVertexBufferData);

        // Note: using upload heaps to transfer static data like vert buffers is
        // not recommended. Every time the GPU needs it, the upload heap will be
        // marshalled over. Please read up on Default Heap usage. An upload heap
        // is used here for code simplicity and because there are very few verts
        // to actually transfer.
        D3D12_HEAP_PROPERTIES heapProps;
        heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
        heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        heapProps.CreationNodeMask = 1;
        heapProps.VisibleNodeMask = 1;

        D3D12_RESOURCE_DESC vertexBufferResourceDesc;
        vertexBufferResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        vertexBufferResourceDesc.Alignment = 0;
        vertexBufferResourceDesc.Width = vertexBufferSize;
        vertexBufferResourceDesc.Height = 1;
        vertexBufferResourceDesc.DepthOrArraySize = 1;
        vertexBufferResourceDesc.MipLevels = 1;
        vertexBufferResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
        vertexBufferResourceDesc.SampleDesc.Count = 1;
        vertexBufferResourceDesc.SampleDesc.Quality = 0;
        vertexBufferResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        vertexBufferResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

        ThrowIfFailed(mDevice->CreateCommittedResource(
            &heapProps, D3D12_HEAP_FLAG_NONE, &vertexBufferResourceDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
            IID_PPV_ARGS(&mVertexBuffer)));

        // Copy the triangle data to the vertex buffer.
        UINT8* pVertexDataBegin;

        // We do not intend to read from this resource on the CPU.
        D3D12_RANGE readRange;
        readRange.Begin = 0;
        readRange.End = 0;

        ThrowIfFailed(mVertexBuffer->Map(
            0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin)));
        memcpy(pVertexDataBegin, mVertexBufferData, sizeof(mVertexBufferData));
        mVertexBuffer->Unmap(0, nullptr);

        // Initialize the vertex buffer view.
        mVertexBufferView.BufferLocation =
            mVertexBuffer->GetGPUVirtualAddress();
        mVertexBufferView.StrideInBytes = sizeof(Vertex);
        mVertexBufferView.SizeInBytes = vertexBufferSize;
    }

    // Create the index buffer.
    {
        const UINT indexBufferSize = sizeof(mIndexBufferData);

        // Note: using upload heaps to transfer static data like vert buffers is
        // not recommended. Every time the GPU needs it, the upload heap will be
        // marshalled over. Please read up on Default Heap usage. An upload heap
        // is used here for code simplicity and because there are very few verts
        // to actually transfer.
        D3D12_HEAP_PROPERTIES heapProps;
        heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
        heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        heapProps.CreationNodeMask = 1;
        heapProps.VisibleNodeMask = 1;

        D3D12_RESOURCE_DESC vertexBufferResourceDesc;
        vertexBufferResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        vertexBufferResourceDesc.Alignment = 0;
        vertexBufferResourceDesc.Width = indexBufferSize;
        vertexBufferResourceDesc.Height = 1;
        vertexBufferResourceDesc.DepthOrArraySize = 1;
        vertexBufferResourceDesc.MipLevels = 1;
        vertexBufferResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
        vertexBufferResourceDesc.SampleDesc.Count = 1;
        vertexBufferResourceDesc.SampleDesc.Quality = 0;
        vertexBufferResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        vertexBufferResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

        ThrowIfFailed(mDevice->CreateCommittedResource(
            &heapProps, D3D12_HEAP_FLAG_NONE, &vertexBufferResourceDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
            IID_PPV_ARGS(&mIndexBuffer)));

        // Copy the triangle data to the vertex buffer.
        UINT8* pVertexDataBegin;

        // We do not intend to read from this resource on the CPU.
        D3D12_RANGE readRange;
        readRange.Begin = 0;
        readRange.End = 0;

        ThrowIfFailed(mIndexBuffer->Map(
            0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin)));
        memcpy(pVertexDataBegin, mIndexBufferData, sizeof(mIndexBufferData));
        mIndexBuffer->Unmap(0, nullptr);

        // Initialize the vertex buffer view.
        mIndexBufferView.BufferLocation = mIndexBuffer->GetGPUVirtualAddress();
        mIndexBufferView.Format = DXGI_FORMAT_R32_UINT;
        mIndexBufferView.SizeInBytes = indexBufferSize;
    }

    // Create synchronization objects and wait until assets have been uploaded
    // to the GPU.
    {
        mFenceValue = 1;

        // Create an event handle to use for frame synchronization.
        mFenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        if (mFenceEvent == nullptr)
        {
            ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
        }

        // Wait for the command list to execute; we are reusing the same command
        // list in our main loop but for now, we just want to wait for setup to
        // complete before continuing.
        // Signal and increment the fence value.
        const UINT64 fence = mFenceValue;
        ThrowIfFailed(mCommandQueue->Signal(mFence, fence));
        mFenceValue++;

        // Wait until the previous frame is finished.
        if (mFence->GetCompletedValue() < fence)
        {
            ThrowIfFailed(mFence->SetEventOnCompletion(fence, mFenceEvent));
            WaitForSingleObject(mFenceEvent, INFINITE);
        }

        mFrameIndex = mSwapchain->GetCurrentBackBufferIndex();
    }

    //-----------------------------------------------------------------------------
    // Setup commands
    //-----------------------------------------------------------------------------
    // Command list allocators can only be reset when the associated
    // command lists have finished execution on the GPU; apps should use
    // fences to determine GPU execution progress.
    ThrowIfFailed(mCommandAllocator->Reset());

    // However, when ExecuteCommandList() is called on a particular command
    // list, that command list can then be reset at any time and must be before
    // re-recording.
    ThrowIfFailed(mCommandList->Reset(mCommandAllocator, mPipelineState));

    // Set necessary state.
    mCommandList->SetGraphicsRootSignature(mRootSignature);
    mCommandList->RSSetViewports(1, &mViewport);
    mCommandList->RSSetScissorRects(1, &mSurfaceSize);

    ID3D12DescriptorHeap* pDescriptorHeaps[] = { mUniformBufferHeap };
    mCommandList->SetDescriptorHeaps(_countof(pDescriptorHeaps),
        pDescriptorHeaps);

    D3D12_GPU_DESCRIPTOR_HANDLE srvHandle(
        mUniformBufferHeap->GetGPUDescriptorHandleForHeapStart());
    mCommandList->SetGraphicsRootDescriptorTable(0, srvHandle);

    // Indicate that the back buffer will be used as a render target.
    D3D12_RESOURCE_BARRIER renderTargetBarrier;
    renderTargetBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    renderTargetBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    renderTargetBarrier.Transition.pResource = mRenderTargets[mFrameIndex];
    renderTargetBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    renderTargetBarrier.Transition.StateAfter =
        D3D12_RESOURCE_STATE_RENDER_TARGET;
    renderTargetBarrier.Transition.Subresource =
        D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

    mCommandList->ResourceBarrier(1, &renderTargetBarrier);

    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle(
        mRtvHeap->GetCPUDescriptorHandleForHeapStart());
    rtvHandle.ptr = rtvHandle.ptr + (mFrameIndex * mRtvDescriptorSize);
    mCommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

    // Record commands.
    const float clearColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    mCommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
    mCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    mCommandList->IASetVertexBuffers(0, 1, &mVertexBufferView);
    mCommandList->IASetIndexBuffer(&mIndexBufferView);

    mCommandList->DrawIndexedInstanced(3, 1, 0, 0, 0);

    // Indicate that the back buffer will now be used to present.
    D3D12_RESOURCE_BARRIER presentBarrier;
    presentBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    presentBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    presentBarrier.Transition.pResource = mRenderTargets[mFrameIndex];
    presentBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    presentBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
    presentBarrier.Transition.Subresource =
        D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

    mCommandList->ResourceBarrier(1, &presentBarrier);

    ThrowIfFailed(mCommandList->Close());

    tStart = std::chrono::high_resolution_clock::now();

    //-----------------------------------------------------------------------------
    // init window
    //-----------------------------------------------------------------------------


    // Main Loop
    bool isRunning = true;
    while (isRunning)
    {
        MSG msg = {};
        while (PeekMessageW(&msg, 0, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                isRunning = false;
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }

        // Framelimit set to 60 fps
        tEnd = std::chrono::high_resolution_clock::now();
        float time =
            std::chrono::duration<float, std::milli>(tEnd - tStart).count();
        if (time < (1000.0f / 60.0f))
        {
            //return;
        }
        tStart = std::chrono::high_resolution_clock::now();

        {
            // Update Uniforms
            mElapsedTime += 0.001f * time;
            mElapsedTime = fmodf(mElapsedTime, 6.283185307179586f);

            uboVS.modelMatrix = glm::rotate(
                uboVS.modelMatrix, 0.001f * time, vec3(0.0f, 1.0f, 0.0f));

            D3D12_RANGE readRange;
            readRange.Begin = 0;
            readRange.End = 0;

            ThrowIfFailed(mUniformBuffer->Map(
                0, &readRange,
                reinterpret_cast<void**>(&mMappedUniformBuffer)));
            memcpy(mMappedUniformBuffer, &uboVS, sizeof(uboVS));
            mUniformBuffer->Unmap(0, &readRange);
        }

        // Record all the commands we need to render the scene into the
        // command list.
        // Command list allocators can only be reset when the associated
        // command lists have finished execution on the GPU; apps should use
        // fences to determine GPU execution progress.
        ThrowIfFailed(mCommandAllocator->Reset());

        // However, when ExecuteCommandList() is called on a particular
        // command list, that command list can then be reset at any time and
        // must be before re-recording.
        ThrowIfFailed(
            mCommandList->Reset(mCommandAllocator, mPipelineState));

        // Set necessary state.
        mCommandList->SetGraphicsRootSignature(mRootSignature);
        mCommandList->RSSetViewports(1, &mViewport);
        mCommandList->RSSetScissorRects(1, &mSurfaceSize);

        ID3D12DescriptorHeap* pDescriptorHeaps[] = { mUniformBufferHeap };
        mCommandList->SetDescriptorHeaps(_countof(pDescriptorHeaps),
            pDescriptorHeaps);

        D3D12_GPU_DESCRIPTOR_HANDLE srvHandle(
            mUniformBufferHeap->GetGPUDescriptorHandleForHeapStart());
        mCommandList->SetGraphicsRootDescriptorTable(0, srvHandle);

        // Indicate that the back buffer will be used as a render target.
        D3D12_RESOURCE_BARRIER renderTargetBarrier;
        renderTargetBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        renderTargetBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        renderTargetBarrier.Transition.pResource =
            mRenderTargets[mFrameIndex];
        renderTargetBarrier.Transition.StateBefore =
            D3D12_RESOURCE_STATE_PRESENT;
        renderTargetBarrier.Transition.StateAfter =
            D3D12_RESOURCE_STATE_RENDER_TARGET;
        renderTargetBarrier.Transition.Subresource =
            D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

        mCommandList->ResourceBarrier(1, &renderTargetBarrier);

        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle(
            mRtvHeap->GetCPUDescriptorHandleForHeapStart());
        rtvHandle.ptr = rtvHandle.ptr + (mFrameIndex * mRtvDescriptorSize);
        mCommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

        // Record commands.
        const float clearColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };
        mCommandList->ClearRenderTargetView(rtvHandle, clearColor, 0,
            nullptr);
        mCommandList->IASetPrimitiveTopology(
            D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        mCommandList->IASetVertexBuffers(0, 1, &mVertexBufferView);
        mCommandList->IASetIndexBuffer(&mIndexBufferView);

        mCommandList->DrawIndexedInstanced(3, 1, 0, 0, 0);

        // Indicate that the back buffer will now be used to present.
        D3D12_RESOURCE_BARRIER presentBarrier;
        presentBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        presentBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        presentBarrier.Transition.pResource = mRenderTargets[mFrameIndex];
        presentBarrier.Transition.StateBefore =
            D3D12_RESOURCE_STATE_RENDER_TARGET;
        presentBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
        presentBarrier.Transition.Subresource =
            D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

        mCommandList->ResourceBarrier(1, &presentBarrier);

        ThrowIfFailed(mCommandList->Close());

        // Execute the command list.
        ID3D12CommandList* ppCommandLists[] = { mCommandList };
        mCommandQueue->ExecuteCommandLists(_countof(ppCommandLists),
            ppCommandLists);
        mSwapchain->Present(1, 0);

        // WAITING FOR THE FRAME TO COMPLETE BEFORE CONTINUING IS NOT BEST
        // PRACTICE.

        // Signal and increment the fence value.
        const UINT64 fence = mFenceValue;
        ThrowIfFailed(mCommandQueue->Signal(mFence, fence));
        mFenceValue++;

        // Wait until the previous frame is finished.
        if (mFence->GetCompletedValue() < fence)
        {
            ThrowIfFailed(mFence->SetEventOnCompletion(fence, mFenceEvent));
            WaitForSingleObject(mFenceEvent, INFINITE);
        }

        mFrameIndex = mSwapchain->GetCurrentBackBufferIndex();

    }

    return 0;
}