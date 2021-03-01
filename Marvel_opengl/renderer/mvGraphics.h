#pragma once
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

namespace Marvel {

	class mvGraphics
	{

	public:

		mvGraphics(GLFWwindow* handle, int width, int height);

		void swapBuffers();

	private:

		GLFWwindow* m_handle;

	};

}