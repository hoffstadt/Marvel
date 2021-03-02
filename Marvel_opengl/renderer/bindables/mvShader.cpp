#include "mvShader.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include "mvGraphics.h"


namespace Marvel {

	static void ReadFile(const char* path, std::string& dst)
	{
		std::string fullpath = "../../../Marvel_opengl/shaders/" + std::string(path);
		std::ifstream stream(fullpath);
		if (stream)
		{
			std::ostringstream ss;
			ss << stream.rdbuf();
			dst = ss.str();
		}
	}

	static unsigned int CompileShader(unsigned int type, const std::string& source)
	{
		unsigned int id = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE)
		{
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*)alloca(length * sizeof(char));
			glGetShaderInfoLog(id, length, &length, message);
			std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
			std::cout << message << std::endl;
			glDeleteShader(id);
			return 0;
		}

		return id;
	}

	mvShader::mvShader(mvGraphics& graphics, const char* vs_path, const char* ps_path)
	{
		std::string VertexSource;
		std::string FragmentSource;

		ReadFile(vs_path, VertexSource);
		ReadFile(ps_path, FragmentSource);

		m_program = glCreateProgram();
		unsigned int vs = CompileShader(GL_VERTEX_SHADER, VertexSource);
		unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, FragmentSource);

		glAttachShader(m_program, vs);
		glAttachShader(m_program, fs);
		glLinkProgram(m_program);
		glValidateProgram(m_program);

		glDeleteShader(vs);
		glDeleteShader(fs);

		glUseProgram(0);

	}

	mvShader::~mvShader()
	{
		glDeleteShader(m_program);
	}

	void mvShader::bind(mvGraphics& graphics)
	{
		glUseProgram(m_program);
	}

}