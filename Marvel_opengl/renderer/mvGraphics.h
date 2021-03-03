#pragma once
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include "mvMath.h"

namespace Marvel {

	class mvGraphics
	{

	public:

		mvGraphics(GLFWwindow* handle, int width, int height);

		void      swapBuffers      ();
		void      drawIndexed      (int count);
		void      setProjection    (glm::mat4 proj);
		void      setCamera        (glm::mat4 cam);
		glm::mat4 getProjection    () const;
		glm::mat4 getCamera        () const;
		void      setCurrentProgram(GLuint program);
		GLuint    getCurrentProgram() const;

	private:

		GLFWwindow* m_handle;
		glm::mat4   m_projection;
		glm::mat4   m_camera;
		GLuint      m_program; // current program

	};

}