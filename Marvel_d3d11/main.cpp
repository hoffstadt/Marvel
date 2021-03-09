#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "mvWindow.h"
#include "mvGraphics.h"
#include "mvImGuiManager.h"
#include "mvCommonBindables.h"

using namespace Marvel;

int main()
{

    int width = 1500;
    int height = 835;

    // create window
    mvWindow window("Marvel", width, height);

    // create graphics
    mvGraphics graphics(window.getWin32Handle(), width, height);

    // create imgui manager
    mvImGuiManager imManager(window.getWin32Handle(), graphics);

    // Create Vertex Shader
    mvVertexShader vertexShader(graphics, "../../../Marvel_d3d11/shaders/vs_texture.hlsl");

    // Create Pixel Shader
    mvPixelShader pixelShader(graphics, "../../../Marvel_d3d11/shaders/ps_texture.hlsl");

    // Create Topology
    mvTopology topology(graphics, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    mvVertexLayout vl;
    vl.append(ElementType::Position2D);
    vl.append(ElementType::Texture2D);

    // create vertex buffer
    mvVertexBuffer vertexBuffer(graphics, 
        {
            -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, 0.5f, 1.0f, 0.0f
        }, vl);

    // create input layout
    mvInputLayout inputLayout(graphics, vl, vertexShader);

    // create Index Buffer
    mvIndexBuffer indexBuffer(graphics,
        std::vector<unsigned short>{
            0, 1, 2,
            0, 3, 1
            }
    );

    // create sampler
    mvSampler sampler(graphics);

    // create texture
    mvTexture texture(graphics, "../../../Resources/SpriteMapExample.png");

    // create constant buffer
    struct Constants // needs to be 16 bit aligned
    {
        DirectX::XMFLOAT4 color;
    };

    Constants cbuffer;
    cbuffer.color = { 1.0f, 0.0f, 0.0f, 1.0f };
    mvPixelConstantBuffer<Constants> pixelConstBuffer(graphics, cbuffer);

    // Main Loop
    while (window.isRunning())
    {

        imManager.beginFrame();

        pixelConstBuffer.update(graphics, cbuffer);
        graphics.getTarget()->clear(graphics);
        graphics.getTarget()->bindAsBuffer(graphics);

        

        // imgui controls
        ImGui::Begin("Settings");
        ImGui::ColorEdit4("Color", (float*)&cbuffer);
        ImGui::End();

        texture.bind(graphics);
        sampler.bind(graphics);
        pixelConstBuffer.bind(graphics);
        inputLayout.bind(graphics);
        topology.bind(graphics);
        indexBuffer.bind(graphics);
        vertexBuffer.bind(graphics);
        vertexShader.bind(graphics);
        pixelShader.bind(graphics);

        graphics.getContext()->DrawIndexed(indexBuffer.getCount(), 0u, 0u);

        imManager.endFrame();

        graphics.getSwapChain()->Present(1, 0);

        window.processEvents();
    }

    return 0;
}