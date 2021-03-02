#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "mvWindow.h"
#include "mvGraphics.h"

#include "mvQuad.h"

using namespace Marvel;

int main(void)
{
	int width = 640;
	int height = 480;
	auto window = mvWindow("Marvel - OpenGL", width, height);
	auto graphics = mvGraphics(window.getHandle(), width, height);

	auto quad = mvQuad(graphics);

	/* Loop until the user closes the window */
	while (window.isRunning())
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		glEnable(GL_CULL_FACE);

		quad.draw(graphics);

		graphics.swapBuffers();

		window.processEvents();
	}

}