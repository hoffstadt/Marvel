#import <Metal/Metal.h>
#import <QuartzCore/QuartzCore.h>
#import "MetalAdder.h"

#define GLFW_INCLUDE_NONE
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

//-----------------------------------------------------------------------------
// Variables
//-----------------------------------------------------------------------------
GLFWwindow*              g_window;
CAMetalLayer*            g_layer;

int main(int argc, const char * argv[]) {
    @autoreleasepool {

        id<MTLDevice> device = MTLCreateSystemDefaultDevice();

        // Create the custom object used to encapsulate the Metal code.
        // Initializes objects to communicate with the GPU.
        MetalAdder* adder = [[MetalAdder alloc] initWithDevice:device];

        // Create buffers to hold data
        [adder prepareData];

        // Send a command to the GPU to perform the calculation.
        [adder sendComputeCommand];

        NSLog(@"Execution finished");
    }
}