#include "mvMath.h"
#include "mvWindow.h"
#include "mvGraphics.h"
#include "mvImGuiManager.h"
#include "mvCommonBindables.h"
#include "mvCommonDrawables.h"
#include "mvCamera.h"
#include "mvTimer.h"
#include "mvPointLight.h"
#include "mvModel.h"
#include "mvRenderGraph.h"

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
    mvRenderGraph graph(graphics);

    // create point light
    mvPointLight light(graphics, {10.0f, 5.0f, 0.0f});

    // create camera
    mvCamera camera1(graphics, {-13.5f, 6.0f, 3.5f}, 0.0f, PI / 2.0f);
    mvCamera camera2(graphics, { 0.0f, 5.0f, -5.0f }, 0.0f, 0.0f, 1.0f, 1.0f);

    // create model
    mvModel model1(graphics, "../../Resources/Models/Sponza/sponza.obj", 1.0f/20.0f);
    mvModel model2(graphics, "../../Resources/Models/gobber/GoblinX.obj", 1.0f);
    model2.setRootTransform(glm::translate(glm::vec3{ 0.0f, 5.0f, 0.0f }));

    model1.linkTechniques(graph);
    light.linkTechniques(graph);
    model2.linkTechniques(graph);

    mvRenderTarget target1(graphics, 1500, 835);
    mvRenderTarget target2(graphics, 300, 300);

    // timer
    Marvel::mvTimer timer;

    // Main Loop
    while (true)
    {
        // process all messages pending, but to not block for new messages
        if (const auto ecode = mvWindow::ProcessMessages())
            break;

        const auto dt = timer.mark() * 1.0f;

        HandleEvents(window, dt, camera1);

        imManager.beginFrame();

        // first render
        target1.bindAsBuffer(graphics);
        target1.clear(graphics);
 
        camera1.bind(graphics);
        light.bind(graphics, camera1.getMatrix());
        
        model1.submit(graph);
        model2.submit(graph);
        light.submit(graph);

        graph.execute(graphics);
        graph.reset();

        // second render
        target2.bindAsBuffer(graphics);
        target2.clear(graphics);

        camera2.bind(graphics);
        light.bind(graphics, camera2.getMatrix());

        model1.submit(graph);
        model2.submit(graph);
        light.submit(graph);

        graph.execute(graphics);
        graph.reset();

        // main frame buffer
        graphics.getTarget()->bindAsBuffer(graphics);
        graphics.getTarget()->clear(graphics);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
        ImGui::SetNextWindowSize(ImVec2(1850.0f, 900.0f));
        if (ImGui::Begin("Main", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar))
        {
            ImGuiID dockspace_id = ImGui::GetID("Main");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
        }
        ImGui::End();
        ImGui::PopStyleVar();

        light.show_imgui_windows("Light 1");

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
        ImGui::SetNextWindowSize(ImVec2(1500.0f, 855.0f));
        if (ImGui::Begin("Render1", nullptr))
        {
            if (target1.getTarget())
                ImGui::Image(target1.getShaderResource(), ImVec2(1500, 835));
        }
        ImGui::End();

        ImGui::SetNextWindowSize(ImVec2(300, 320));
        if (ImGui::Begin("Render2", nullptr))
        {
            if (target2.getTarget())
                ImGui::Image(target2.getShaderResource(), ImVec2(300, 300));
        }
        ImGui::End();
        ImGui::PopStyleVar();

        ImGuiIO& io = ImGui::GetIO();
        ImGui::GetForegroundDrawList()->AddText(ImVec2(375, 45),
            ImColor(0.0f, 1.0f, 0.0f), std::string(std::to_string(io.Framerate) + " FPS").c_str());
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