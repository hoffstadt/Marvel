#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "mvWindow.h"
#include "mvGraphics.h"
#include "mvQuad.h"
#include "mvTexturedQuad.h"
#include "mvCamera.h"

using namespace Marvel;

int width = 640;
int height = 480;
auto window = mvWindow("Marvel - OpenGL", width, height);
auto graphics = mvGraphics(window.getHandle(), width, height);
auto quad = mvTexturedQuad(graphics, "../../../Resources/SpriteMapExample.png");
auto quad1 = mvQuad(graphics);
auto camera = mvCamera(graphics);
bool free_mode = false;

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	static double old_xpos = xpos;
	static double old_ypos = ypos;
	double dx = old_xpos - xpos;
	double dy = old_ypos - ypos;
	camera.rotate( dy, dx);
	old_xpos = xpos;
	old_ypos = ypos;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_A && glfwGetKey(window, key) == GLFW_PRESS)
		camera.translate(-0.05, 0, 0);
	if (key == GLFW_KEY_D && action == GLFW_PRESS)
		camera.translate(0.05, 0, 0);
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
		camera.translate(0, 0, -0.05);
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
		camera.translate(0, 0, 0.05);

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		free_mode = !free_mode;
		if (free_mode)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
			glfwSetCursorPosCallback(window, cursor_position_callback);
		}
		else
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
			glfwSetCursorPosCallback(window, nullptr);
		}
	}
}

int main(void)
{

	quad.setPosition(0, 0, -5);

	glfwSetKeyCallback(window.getHandle(), key_callback);

	/* Loop until the user closes the window */
	while (window.isRunning())
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		//glEnable(GL_CULL_FACE);

		camera.bind(graphics);

		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;

		quad.bind(graphics);
		quad.draw(graphics);

		quad1.bind(graphics);
		quad1.draw(graphics);

		graphics.swapBuffers();

		window.processEvents();
	}

}