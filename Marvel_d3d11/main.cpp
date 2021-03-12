#include "mvMath.h"
#include "mvWindow.h"
#include "mvGraphics.h"
#include "mvImGuiManager.h"
#include "mvCommonBindables.h"
#include "mvCommonDrawables.h"
#include "mvCamera.h"
#include "mvTimer.h"
#include "mvPointLight.h"

using namespace Marvel;

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

    // create point light
    Marvel::mvPointLight light(graphics);

    // create textured quad
    mvTexturedQuad tquad(graphics, "../../../Resources/SpriteMapExample.png");
    tquad.setPosition(0.0f, 0.0f, 5.0f);

    // create solid sphere phong
    mvSolidSphere sphere1(graphics, 0);
    sphere1.setPosition(2.0f, 0.0f, 5.0f);

    // create solid sphere
    mvSolidSphere sphere2(graphics, 2);
    sphere2.setPosition(-2.0f, 0.0f, 5.0f);

    // create camera
    mvCamera camera(graphics);

    // timer
    Marvel::mvTimer timer;

    // Main Loop
    while (true)
    {
        // process all messages pending, but to not block for new messages
        if (const auto ecode = mvWindow::ProcessMessages())
            break;

        const auto dt = timer.mark() * 1.0f;

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
                camera.translate(0.0f, 0.0f, dt );

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

        imManager.beginFrame();

        graphics.getTarget()->clear(graphics);
        graphics.getTarget()->bindAsBuffer(graphics);

        // bind camera
        camera.bind(graphics);

        // bind light
        light.bind(graphics, camera.getMatrix());
        light.show_imgui_windows();
        light.getSphere()->draw(graphics);
        
        tquad.draw(graphics);
        tquad.show_imgui_windows("Textured Quad");
        
        sphere1.draw(graphics);
        sphere1.show_imgui_windows("Phong Shaded Sphere");
        
        sphere2.draw(graphics);
        sphere2.show_imgui_windows("Flat Shaded Sphere");

        imManager.endFrame();

        graphics.getSwapChain()->Present(1, 0);

    }

}