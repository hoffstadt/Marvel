#pragma once
#include "mvMarvelWin.h"
#include <GLFW/glfw3.h>

namespace Marvel {

	class mvWindow
	{

	public:

		mvWindow(const char* name, int width, int height);
		~mvWindow();

		void processEvents();

		GLFWwindow* getHandle() const { return m_handle; }
		HWND getWin32Handle() const;
		bool isRunning() const { return m_running; }


	private:

		int         m_width;
		int         m_height;
		GLFWwindow* m_handle;
		bool        m_running;

	};

}