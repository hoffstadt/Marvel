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

	void mvGraphics::drawIndexed(int count)
	{
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, 0);
	}

	void mvGraphics::setProjection(glm::mat4 proj)
	{
		m_projection = proj;
	}

	void mvGraphics::setCamera(glm::mat4 cam)
	{
		m_camera = cam;
	}

	glm::mat4 mvGraphics::getProjection() const
	{
		return m_projection;
	}

	glm::mat4 mvGraphics::getCamera() const
	{
		return m_camera;
	}
	
}