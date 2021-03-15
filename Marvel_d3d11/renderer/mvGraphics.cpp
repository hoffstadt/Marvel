#include "mvGraphics.h"
#include <assert.h>
#include "mvCommonBindables.h"

namespace Marvel {

	mvGraphics::mvGraphics(HWND hwnd, int width, int height)
	{
        DXGI_SWAP_CHAIN_DESC sd = {};
        sd.BufferDesc.Width = width;
        sd.BufferDesc.Height = height;
        sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 0;
        sd.BufferDesc.RefreshRate.Denominator = 0;
        sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.BufferCount = 2;
        sd.OutputWindow = hwnd;
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
            &m_swapChain,
            &m_device,
            nullptr,
            &m_deviceContext
        );
        assert(SUCCEEDED(hResult));

        // Create Framebuffer Render Target
        hResult = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)m_frameBuffer.GetAddressOf());
        assert(SUCCEEDED(hResult));

        m_target = new mvRenderTarget(*this, m_frameBuffer.Get());

        mvBindableRegistry::Initialize(*this);
        
	}

    mvGraphics::~mvGraphics()
    {
        delete m_target;
        m_target = nullptr;
    }

    void mvGraphics::drawIndexed(UINT count)
    {
        m_deviceContext->DrawIndexed(count, 0u, 0u);
    }

    void mvGraphics::setProjection(glm::mat4 proj)
    {
        m_projection = proj;
    }

    void mvGraphics::setCamera(glm::mat4 cam)
    {
        m_camera = cam;
    }

    ID3D11Device* mvGraphics::getDevice() 
    { 
        return m_device.Get(); 
    };

    ID3D11DeviceContext* mvGraphics::getContext() 
    { 
        return m_deviceContext.Get(); 
    }

    IDXGISwapChain* mvGraphics::getSwapChain() 
    {
        return m_swapChain.Get(); 
    }

    ID3D11Texture2D* mvGraphics::getFrameBuffer() 
    { 
        return m_frameBuffer.Get(); 
    }

    mvRenderTarget* mvGraphics::getTarget() 
    { 
        return m_target; 
    }

    glm::mat4 mvGraphics::getProjection() const
    {
        return m_projection;
    }

    glm::mat4 mvGraphics::getCamera() const
    {
        return m_camera;
    }

}