#include "mvPBRMaterialCBuf.h"
#include "mvDrawable.h"
#include "mvGraphics.h"
#include "mvDrawable.h"

namespace Marvel {

	mvPBRMaterialCBuf::mvPBRMaterialCBuf(mvGraphics& graphics, UINT slot)
	{
		m_buf = std::make_unique<mvPixelConstantBuffer>(graphics, slot, &material);
	}

	void mvPBRMaterialCBuf::bind(mvGraphics& graphics)
	{
		m_buf->update(graphics, material);
		m_buf->bind(graphics);
	}

}