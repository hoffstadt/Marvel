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

		void                  bind     (mvGraphics& gfx) override;
		const mvVertexLayout& GetLayout() const;

	protected:

		UINT                   m_stride;
		mvComPtr<ID3D11Buffer> m_vertexBuffer;
		mvVertexLayout         m_layout;
	};
}
