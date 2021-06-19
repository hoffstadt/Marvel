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

	void mvPhongMaterialCBuf::showControls()
	{
		ImGui::PushID(this);
		ImGui::ColorEdit3("Material Color", &material.materialColor.x);
		ImGui::ColorEdit3("Specular Color", &material.specularColor.x);
		ImGui::SliderFloat("Normal Weight", &material.normalMapWeight, 0.0f, 1.0f);
		ImGui::Checkbox("Use Color Map", (bool*)&material.useTextureMap);
		ImGui::Checkbox("Use Normal Map", (bool*)&material.useNormalMap);
		ImGui::Checkbox("Use Specular Map", (bool*)&material.useSpecularMap);
		ImGui::PopID();
	}

}