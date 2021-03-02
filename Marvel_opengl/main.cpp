#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "mvWindow.h"
#include "mvGraphics.h"
#include "mvShader.h"
#include "mvIndexBuffer.h"
#include "mvVertexBuffer.h"
#include "mvVertexLayout.h"
#include "mvInputLayout.h"

using namespace Marvel;

int main(void)
{
	int width = 640;
	int height = 480;
	auto window = mvWindow("Hello Triangle - OpenGL", width, height);
	auto graphics = mvGraphics(window.getHandle(), width, height);

	mvVertexLayout vl;
	vl.Append(ElementType::Position2D);

	mvDynamicVertexBuffer dvertexBuffer(std::move(vl));
	dvertexBuffer.EmplaceBack(
		std::array{ -0.5f, -0.5f }
	);

	dvertexBuffer.EmplaceBack(
		std::array{ 0.0f,  0.5f }
	);

	dvertexBuffer.EmplaceBack(
		std::array{ 0.5f, -0.5f }
	);

	auto index_buffer = mvIndexBuffer(graphics, { 2, 1, 0 });

	// create vertex buffer
	auto vertex_buffer = mvVertexBuffer(graphics, dvertexBuffer);

	auto shader = mvShader(graphics, "vs_simple.glsl", "ps_simple.glsl");

	// create input layout
	mvInputLayout inputLayout(graphics, dvertexBuffer.GetLayout(), shader);

	/* Loop until the user closes the window */
	while (window.isRunning())
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		glEnable(GL_CULL_FACE);

		shader.bind(graphics);
		inputLayout.bind(graphics);
		index_buffer.bind(graphics);
		vertex_buffer.bind(graphics);
		
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0);

		graphics.swapBuffers();

		window.processEvents();
	}

}
