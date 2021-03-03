#pragma once
#include "mvBindable.h"
#include <GL/gl3w.h>

namespace Marvel {

	class mvShader : public mvBindable
	{

	public:

		mvShader(mvGraphics& graphics, const char* vs_path, const char* ps_path);
		~mvShader();

		void bind(mvGraphics& graphics) override;

		GLuint getProgram() const;

	private:

		GLuint m_program;

	};

}