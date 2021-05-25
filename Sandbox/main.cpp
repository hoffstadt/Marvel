#include <imgui_impl_dx11.h>
#include "mvMath.h"
#include "mvWindow.h"
#include "mvGraphics.h"
#include "mvCommonBindables.h"
#include "mvCommonDrawables.h"
#include "mvPointShadowMappingPass.h"
#include "mvLambertianPass.h"
#include "mvCamera.h"
#include "mvTimer.h"
#include "mvPointLight.h"
#include "mvDirectionLight.h"
#include "mvModel.h"
#include "mvRenderGraph.h"
#include "mvModelProbe.h"
#include "mvComputeShader.h"

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

    // create render graph
    auto graph = std::make_unique<mvRenderGraph>(graphics, "../../Resources/SkyBox");

    auto directionLight = mvDirectionLight(graphics, { 0.0f, -1.0f, 0.0f });

    mvPointLight pointlight(graphics, "light0", { 0.0f, 5.0f, 0.0f });
    auto lightcamera = pointlight.getCamera();

    // create camera
    mvCamera camera(graphics, "maincamera", { -13.5f, 6.0f, 3.5f }, 0.0f, PI / 2.0f, width, height, 0.5f, 400.0f, false);
    //mvCamera directionCamera(graphics, "directionCamera", { 0.0f, 75.0f, 0.0f }, PI / 2.0f, 0.0f, 300, 300, 0.5f, 100.0f, true);

    // create model
    mvModel model(graphics, "../../Resources/Models/Sponza/sponza.obj", 1.0f);
    //mvModel model(graphics, "../../Resources/Models/gobber/GoblinX.obj", 1.0f);
    //mvSolidSphere model(graphics, 1.0f, { 1.0f, 0.2f, 0.0f }, 0);

    // create testing cube
    mvCube cube(graphics, "testcube", { 1.0f, 0.0f, 0.5f });
    cube.setPosition(0.0f, 5.0f, 10.0f);

    // create testing quad
    mvTexturedQuad quad(graphics, "testquad", "../../Resources/brickwall.jpg");
    quad.setPosition(5.0f, 5.0f, 10.0f);


    // testing compute shaders
    struct BufType { float f;};
    BufType* inputRawBuffer = new BufType[1024];

    for (int i = 0; i < 1024; ++i)
        inputRawBuffer[i].f = 1.5f;

    mvComputeShader computeShader(graphics, graphics.getShaderRoot() + "testcompute.hlsl");
    mvComputeInputBuffer<BufType> inputBuffer(graphics, 0, inputRawBuffer, 1024);
    mvComputeOutputBuffer<BufType> outputBuffer(graphics, 0, 1024);

    inputBuffer.bind(graphics);
    outputBuffer.bind(graphics);
    computeShader.dispatch(graphics, 1024, 1, 1);

    BufType* p = outputBuffer.getDataFromGPU(graphics);

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
            graph.reset();

            graphics.releaseBuffers();
            graphics.resize(window.getClientWidth(), window.getClientHeight());
            camera.updateProjection(window.getClientWidth(), window.getClientHeight());
            window.setResizedFlag(false);

            graph = std::make_unique<mvRenderGraph>(graphics, "../../Resources/SkyBox");
            model.linkTechniques(*graph);
            cube.linkTechniques(*graph);
            quad.linkTechniques(*graph);
            pointlight.linkTechniques(*graph);
            camera.linkTechniques(*graph);
            lightcamera->linkTechniques(*graph);
            //directionCamera.linkTechniques(*graph);
            static_cast<mvLambertianPass*>(graph->getPass("lambertian"))->bindShadowCamera(*pointlight.getCamera());
            static_cast<mvPointShadowMappingPass*>(graph->getPass("shadow"))->bindShadowCamera(*pointlight.getCamera());
        }

        const auto dt = timer.mark() * 1.0f;

        HandleEvents(window, dt, camera);

        graphics.beginFrame();

        graph->bindMainCamera(camera);
        static_cast<mvLambertianPass*>(graph->getPass("lambertian"))->bindShadowCamera(*pointlight.getCamera());
        static_cast<mvPointShadowMappingPass*>(graph->getPass("shadow"))->bindShadowCamera(*pointlight.getCamera());

        graph->bind(graphics);

        pointlight.bind(graphics, camera.getMatrix());
        directionLight.bind(graphics, camera.getMatrix());

        cube.submit(*graph);
        quad.submit(*graph);
        model.submit(*graph);
        pointlight.submit(*graph);
        //lightcamera->submit(*graph);
        //directionCamera.submit(*graph);

        graph->execute(graphics);

        static mvModelProbe probe(graphics, "Model Probe");

        probe.spawnWindow(model);
        pointlight.show_imgui_window();
        directionLight.show_imgui_window();
        graph->show_imgui_window();
        cube.show_imgui_windows("Test Cube");
        quad.show_imgui_windows("Test Quad");

        ImGuiIO& io = ImGui::GetIO();
        ImGui::GetForegroundDrawList()->AddText(ImVec2(45, 45),
            ImColor(0.0f, 1.0f, 0.0f), std::string(std::to_string(io.Framerate) + " FPS").c_str());

        ////ImGui::SetNextWindowSize(ImVec2(300, 320));
        //if (ImGui::Begin("Light Camera", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        //{
        //    //if (target1.getTarget())
        //        ImGui::Image(static_cast<mvShadowMappingPass*>(graph->getPass("shadow"))->getOutput()->getShaderResource(), ImVec2(300, 300));
        //}
        //ImGui::End();

        graph->reset();

        graphics.endFrame();

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