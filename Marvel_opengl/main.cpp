#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "mvWindow.h"
#include "mvGraphics.h"

#include "mvQuad.h"
#include "mvUniform.h"

using namespace Marvel;

int main(void)
{
	int width = 640;
	int height = 480;
	auto window = mvWindow("Marvel - OpenGL", width, height);
	auto graphics = mvGraphics(window.getHandle(), width, height);

	auto quad = mvQuad(graphics);

	auto uniform = mvUniformVector(graphics, std::array{ 0.0f, 1.0f, 0.0f, 1.0f }, 4);

	/* Loop until the user closes the window */
	while (window.isRunning())
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		glEnable(GL_CULL_FACE);

		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		uniform.update(std::array{ 0.0f, greenValue, 0.0f, 1.0f });

		quad.bind(graphics);
		uniform.bind(graphics);
		quad.draw(graphics);

		graphics.swapBuffers();

		window.processEvents();
	}

}