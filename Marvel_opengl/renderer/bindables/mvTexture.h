#pragma once
#include <string>
#include "mvBindable.h"
#include <GL/gl3w.h>

namespace Marvel {

	class mvTexture : public mvBindable
	{

	public:

		mvTexture(mvGraphics& graphics, const std::string& path, GLenum slot = GL_TEXTURE0);

		void bind(mvGraphics& graphics) override;

	private:

		GLuint m_texture;
		GLenum m_slot;

	};

}