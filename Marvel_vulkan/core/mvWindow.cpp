#include "mvWindow.h"
namespace Marvel {

	mvWindow::mvWindow(const char* name, int width, int height)
		:
		m_width(width),
		m_height(height),
		m_handle(nullptr),
		m_running(true)
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		m_handle = glfwCreateWindow(width, height, name, nullptr, nullptr);

	}

	mvWindow::~mvWindow()
	{
		glfwTerminate();
	}

	void mvWindow::processEvents()
	{
		/* Loop until the user closes the window */
		m_running = !glfwWindowShouldClose(m_handle);

		/* Poll for and process events */
		glfwPollEvents();
	}

}