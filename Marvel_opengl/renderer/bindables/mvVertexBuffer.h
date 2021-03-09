#pragma once
#include <memory>
#include "mvBindable.h"
#include "mvVertexLayout.h"

namespace Marvel
{
	class mvVertexBuffer : public mvBindable
	{

	public:

		mvVertexBuffer(mvGraphics& graphics, const std::vector<float>& vbuf);

		void bind  (mvGraphics& graphics) override;
		void unbind(mvGraphics& graphics) override;

	private:

		GLuint m_buffer;

	};
}
