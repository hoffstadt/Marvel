#include "mvPhongMaterialCBuf.h"
#include "mvDrawable.h"
#include "mvGraphics.h"
#include "mvDrawable.h"

namespace Marvel {

	mvPhongMaterialCBuf::mvPhongMaterialCBuf(mvGraphics& graphics, UINT slot)
	{
		m_buf = std::make_unique<mvPixelConstantBuffer>(graphics, slot, &material);
	}

	void mvPhongMaterialCBuf::bind(mvGraphics& graphics)
	{
		m_buf->update(graphics, material);
		m_buf->bind(graphics);
	}

}