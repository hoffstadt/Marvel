#include "mvGraphics.h"
#include <GL/gl3w.h>
#include <iostream>

namespace Marvel {

	mvGraphics::mvGraphics(GLFWwindow* handle, int width, int height)
		:
		m_handle(handle)
	{
		/* Make the window's context current */
		glfwMakeContextCurrent(m_handle);

		gl3wInit();

		std::cout << glGetString(GL_VERSION) << std::endl;
	}

	void mvGraphics::swapBuffers()
	{
		/* Swap front and back buffers */
		glfwSwapBuffers(m_handle);

	}
	
}