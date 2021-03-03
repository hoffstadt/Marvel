#include "mvGraphics.h"
#include <GL/gl3w.h>
#include <iostream>

namespace Marvel {

	mvGraphics::mvGraphics(GLFWwindow* handle, int width, int height)
		:
		m_handle(handle)
	{
		glfwMakeContextCurrent(m_handle);

		gl3wInit();

		std::cout << glGetString(GL_VERSION) << std::endl;
	}

	void mvGraphics::swapBuffers()
	{
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

	void mvGraphics::setCurrentProgram(GLuint program)
	{
		m_program = program;
	}

	GLuint mvGraphics::getCurrentProgram() const
	{
		return m_program;
	}
	
}