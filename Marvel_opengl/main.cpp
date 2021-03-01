#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "mvWindow.h"
#include "mvGraphics.h"
#include "mvShader.h"

using namespace Marvel;

int main(void)
{
	int width = 640;
	int height = 480;
	auto window = mvWindow("Hello Triangle - OpenGL", width, height);
	auto graphics = mvGraphics(window.getHandle(), width, height);

	float positions[6] = {
		-0.5f, -0.5f,
		 0.0f,  0.5f,
		 0.5f, -0.5f
	};

	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	auto shader = mvShader(graphics, "../../../Marvel_opengl/shaders/vs_simple.glsl", "../../../Marvel_opengl/shaders/vs_simple.glsl");

	/* Loop until the user closes the window */
	while (window.isRunning())
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		shader.bind(graphics);

		graphics.swapBuffers();

		window.processEvents();
	}

}
