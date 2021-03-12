#include "mvMath.h"
#include "mvWindow.h"
#include "mvGraphics.h"
#include "mvImGuiManager.h"
#include "mvCommonBindables.h"
#include "mvCommonDrawables.h"
#include "mvCamera.h"
#include "mvTimer.h"

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

    // create textured quad
    mvTexturedQuad tquad(graphics, "../../../Resources/SpriteMapExample.png");
    tquad.setPosition(0.0f, 0.0f, 10.0f);

    // create solid sphere
    mvSolidSphere sphere1(graphics, 1);

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

        // bind camera
        camera.bind(graphics);

        graphics.getTarget()->clear(graphics);
        graphics.getTarget()->bindAsBuffer(graphics);

        tquad.draw(graphics);

        sphere1.draw(graphics);

        imManager.endFrame();

        graphics.getSwapChain()->Present(1, 0);

    }

}