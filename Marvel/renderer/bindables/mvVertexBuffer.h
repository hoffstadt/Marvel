#pragma once
#include <memory>
#include "mvBindable.h"
#include "mvVertexLayout.h"

namespace Marvel
{
	class mvVertexBuffer : public mvBindable
	{

	public:

		mvVertexBuffer(mvGraphics& gfx, const std::vector<float>& vbuf, const mvVertexLayout& layout);

		void bind (mvGraphics& gfx) override;

		const mvVertexLayout& GetLayout() const;

	private:

		UINT                   m_stride;
		mvComPtr<ID3D11Buffer> m_vertexBuffer;
		mvVertexLayout         m_layout;

	};
}
