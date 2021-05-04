#include <imgui_impl_dx11.h>
#include "mvMath.h"
#include "mvWindow.h"
#include "mvGraphics.h"
#include "mvImGuiManager.h"
#include "mvCommonBindables.h"
#include "mvCommonDrawables.h"
#include "mvCamera.h"
#include "mvTimer.h"
#include "mvPointLight.h"
#include "mvDirectionLight.h"
#include "mvModel.h"
#include "mvRenderGraph.h"
#include "mvModelProbe.h"

using namespace Marvel;

void HandleEvents(mvWindow& window, float dt, mvCamera& camera);

int main()
{

    int width = 1850;
    int height = 900;

    // create window
    mvWindow window("Marvel", width, height);

    // create graphics
    mvGraphics graphics(window.getHandle(), width, height);

    // create imgui manager
    mvImGuiManager imManager(window.getHandle(), graphics);

    // create render graph
    mvRenderGraph graph(graphics, "../../Resources/SkyBox");
    
    mvDirectionLightManager dlightManager(graphics);
    dlightManager.addLight(graphics, { 0.0f, -1.0f, 0.0f });

    mvPointLightManager lightManager(graphics);
    //lightManager.addLight(graphics, { 35.1f, 19.7f, -26.0f });
    //lightManager.addLight(graphics, { 0.0f, 0.0f, 0.0f });
    lightManager.addLight(graphics, { 0.0f, 7.0f, 6.1f });
    auto lightcamera = lightManager.getLight(0).getCamera();

    // create camera
    mvCamera camera(graphics, {-13.5f, 6.0f, 3.5f}, 0.0f, PI / 2.0f, width, height);
    //mvCamera camera(graphics, { 0.0f, 0.0f, -5.0f }, 0.0f, 0.0f, width, height);
    //mvCamera camera(graphics, { 0.0f, 0.0f, -10.0f }, 0.0f, 0.0f, width, height);

    // create model
    mvModel model(graphics, "../../Resources/Models/Sponza/sponza.obj", 1.0f);
    //mvModel model(graphics, "../../Resources/Models/gobber/GoblinX.obj", 1.0f);
    //mvSolidSphere model(graphics, 1.0f, { 1.0f, 0.2f, 0.0f }, 0);

    // create testing cube
    mvCube cube(graphics, { 1.0f, 0.0f, 0.5f });
    cube.setPosition(0.0f, 5.0f, 0.0f);

   
    model.linkTechniques(graph);
    cube.linkTechniques(graph);
    lightManager.linkTechniques(graph);


    // Light target
    mvRenderTarget target1(graphics, 300, 300);
    mvDepthStencil depthBuffer(graphics, 300, 300);

    // timer
    Marvel::mvTimer timer;

    // Main Loop
    while (true)
    {
        // process all messages pending, but to not block for new messages
        if (const auto ecode = mvWindow::ProcessMessages())
            break;

        if (window.wantsResize())
        {
            graphics.resize(window.getClientWidth(), window.getClientHeight());
            camera.updateProjection(window.getClientWidth(), window.getClientHeight());
            window.setResizedFlag(false);
        }

        const auto dt = timer.mark() * 1.0f;

        HandleEvents(window, dt, camera);

        // light pass
        target1.bindAsBuffer(graphics, depthBuffer.getDepthStencilView());
        target1.clear(graphics);
        depthBuffer.clear(graphics);

        graph.bind(graphics);

        lightcamera->bind(graphics);
        lightManager.bind(graphics, lightcamera->getMatrix());
        dlightManager.bind(graphics, lightcamera->getMatrix());

        model.submit(graph);
        cube.submit(graph);
        lightManager.submit(graph);

        graph.execute(graphics);
        graph.reset();

        // viewport pass
        graphics.getTarget()->bindAsBuffer(graphics, graphics.getDepthBuffer()->getDepthStencilView());
        graphics.getTarget()->clear(graphics);
        graphics.getDepthBuffer()->clear(graphics);

        graph.bind(graphics);

        camera.bind(graphics);
        lightManager.bind(graphics, camera.getMatrix());
        dlightManager.bind(graphics, camera.getMatrix());

        cube.submit(graph);
        model.submit(graph);
        lightManager.submit(graph);

        graph.execute(graphics);
        graph.reset();

        static mvModelProbe probe(graphics, "Model Probe");

        imManager.beginFrame();
        probe.spawnWindow(model);
        lightManager.show_imgui_windows();
        dlightManager.show_imgui_windows();
        graph.show_imgui_window();
        cube.show_imgui_windows("Test Cube");

        ImGuiIO& io = ImGui::GetIO();
        ImGui::GetForegroundDrawList()->AddText(ImVec2(45, 45),
            ImColor(0.0f, 1.0f, 0.0f), std::string(std::to_string(io.Framerate) + " FPS").c_str());
        
        //ImGui::SetNextWindowSize(ImVec2(300, 320));
        if (ImGui::Begin("Light Camera", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            if (target1.getTarget())
                ImGui::Image(target1.getShaderResource(), ImVec2(300, 300));
        }
        ImGui::End();

        imManager.endFrame();

        graphics.getSwapChain()->Present(1, 0);

    }

}

void HandleEvents(mvWindow& window, float dt, mvCamera& camera)
{
    while (const auto e = window.kbd.readKey())
    {
        if (!e->isPress())
            continue;

        switch (e->getCode())
        {
        case VK_ESCAPE:
            if (window.cursorEnabled())
            {
                window.disableCursor();
                window.mouse.enableRaw();
            }
            else
            {
                window.enableCursor();
                window.mouse.disableRaw();
            }
            break;
        }
    }

    if (!window.cursorEnabled())
    {
        if (window.kbd.keyIsPressed('W'))
            camera.translate(0.0f, 0.0f, dt);

        if (window.kbd.keyIsPressed('A'))
            camera.translate(-dt, 0.0f, 0.0f);

        if (window.kbd.keyIsPressed('S'))
            camera.translate(0.0f, 0.0f, -dt);

        if (window.kbd.keyIsPressed('D'))
            camera.translate(dt, 0.0f, 0.0f);

        if (window.kbd.keyIsPressed('R'))
            camera.translate(0.0f, dt, 0.0f);

        if (window.kbd.keyIsPressed('F'))
            camera.translate(0.0f, -dt, 0.0f);
    }

    while (const auto delta = window.mouse.readRawDelta())
    {
        if (!window.cursorEnabled())
            camera.rotate((float)delta->x, (float)delta->y);
    }
}