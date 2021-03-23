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

    int width = 1500;
    int height = 835;

    // create window
    mvWindow window("Marvel", width, height);

    // create graphics
    mvGraphics graphics(window.getHandle(), width, height);

    // create imgui manager
    mvImGuiManager imManager(window.getHandle(), graphics);

    // create render graph
    mvRenderGraph graph(graphics);

    // create point light
    //mvPointLight light(graphics, {10.0f, 5.0f, 0.0f});
    mvPointLight light(graphics, {2.5f, 0.0f, -2.5f});

    // create camera
    //mvCamera camera(graphics, {-13.5f, 6.0f, 3.5f}, 0.0f, PI / 2.0f);
    mvCamera camera(graphics, { 0.0f, 0.0f, -5.0f });

    // create model
    //mvModel model(graphics, "../../Resources/Models/Sponza/sponza.obj", 1.0f/20.0f);
    mvModel model(graphics, "../../Resources/Models/gobber/GoblinX.obj", 1.0f);

    model.linkTechniques(graph);
    light.linkTechniques(graph);

    // timer
    Marvel::mvTimer timer;

    // Main Loop
    while (true)
    {
        // process all messages pending, but to not block for new messages
        if (const auto ecode = mvWindow::ProcessMessages())
            break;

        const auto dt = timer.mark() * 1.0f;

        HandleEvents(window, dt, camera);

        imManager.beginFrame();

        graphics.getTarget()->bindAsBuffer(graphics);
        graphics.getTarget()->clear(graphics);
        
        // bind camera
        camera.bind(graphics);

        // bind light
        light.bind(graphics, camera.getMatrix());
        light.show_imgui_windows();
        

        model.submit(graph);
        light.submit(graph);

        graph.execute(graphics);

        ImGuiIO& io = ImGui::GetIO();
        ImGui::GetForegroundDrawList()->AddText(ImVec2(25, 25),
            ImColor(0.0f, 1.0f, 0.0f), std::string(std::to_string(io.Framerate) + " FPS").c_str());
        imManager.endFrame();

        graphics.getSwapChain()->Present(1, 0);

        graph.reset();

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