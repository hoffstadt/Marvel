#pragma once
#include <memory>
#include "mvBindable.h"
#include "mvVertexLayout.h"

namespace Marvel
{
	class mvVertexBuffer : public mvBindable
	{

	public:

		mvVertexBuffer(mvGraphics& gfx, const mvDynamicVertexBuffer& vbuf);

		void bind(mvGraphics& gfx) override;

	private:

		unsigned int m_buffer;

	};
}
