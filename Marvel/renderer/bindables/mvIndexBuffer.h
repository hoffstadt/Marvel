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

		mvIndexBuffer(mvGraphics& graphics, const std::vector<unsigned short>& indices);

		void bind(mvGraphics& graphics) override;

		UINT getCount() const;

	private:

		mvComPtr<ID3D11Buffer> m_indexBuffer;
		UINT                   m_count;

	};
}