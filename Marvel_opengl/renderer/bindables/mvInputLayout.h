#pragma once
#include "mvBindable.h"
#include "mvGraphics.h"
#include "mvShader.h"
#include "mvVertexLayout.h"
#include <vector>

namespace Marvel {

	class mvInputLayout : public mvBindable
	{

	public:

		mvInputLayout(mvGraphics& graphics, mvVertexLayout vertexLayout);

		void bind  (mvGraphics& graphics) override;
		void unbind(mvGraphics& graphics) override;

		// binds attributes to the vertex array object,
		// this is necessary to first bind the vertex
		// buffer and index buffer AFTER binding the
		// vao
		void init(mvShader* shader);

	private:

		mvVertexLayout                      m_layout;
		GLuint                              m_vao;

	};

}