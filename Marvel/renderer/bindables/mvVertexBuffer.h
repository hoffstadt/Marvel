#pragma once
#include <memory>
#include "mvBindable.h"
#include "mvVertexLayout.h"

namespace Marvel
{
	class mvVertexBuffer : public mvBindable
	{

	public:

		mvVertexBuffer(mvGraphics& gfx, const std::vector<float>& vbuf, const mvVertexLayout& layout, bool dynamic = false);

		void bind (mvGraphics& gfx) override;

		void update(mvGraphics& graphics);

		bool isDynamic() const;

		std::vector<float>& getData();

		const mvVertexLayout& GetLayout() const;

	private:

		UINT                   m_stride;
		mvComPtr<ID3D11Buffer> m_vertexBuffer;
		mvVertexLayout         m_layout;
		std::vector<float>     m_data;
		bool                   m_dynamic;
		int                    m_buffersize;

	};
}
