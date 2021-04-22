#pragma once
#include <d3d11.h>
#include <vector>
#include <assert.h>
#include "mvBindable.h"
#include "mvComPtr.h"

namespace Marvel
{
	class mvIndexBuffer : public mvBindable
	{

	public:

		mvIndexBuffer(mvGraphics& graphics, const std::vector<unsigned short>& indices, bool dynamic = false);

		void bind(mvGraphics& graphics) override;

		void update(mvGraphics& graphics);

		bool isDynamic() const;

		std::vector<unsigned short>& getData();

		UINT getCount() const;

	private:

		mvComPtr<ID3D11Buffer>      m_indexBuffer;
		UINT                        m_count;
		std::vector<unsigned short> m_data;
		bool                        m_dynamic;
		int                         m_buffersize;

	};
}