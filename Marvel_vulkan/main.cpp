#include "mvGraphics.h"
#include "mvWindow.h"

using namespace Marvel;

int main() 
{

    auto window = mvWindow("Marvel - Vulkan", 800, 600);

    auto graphics = mvGraphics(window.getHandle());

    // main loop
    while (window.isRunning()) 
    {
        graphics.drawFrame();

        window.processEvents();
    }

}