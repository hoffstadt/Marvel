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

	void mvPBRMaterialCBuf::showControls()
	{
		ImGui::PushID(this);
		ImGui::ColorEdit3("Albedo", &material.albedo.x);
		ImGui::SliderFloat("Metalness", &material.metalness, 0.0f, 1.0f);
		ImGui::SliderFloat("Roughness", &material.roughness, 0.0f, 1.0f);
		ImGui::SliderFloat("Radiance", &material.radiance, 0.0f, 1.0f);
		ImGui::SliderFloat("Fresnel", &material.fresnel, 0.0f, 1.0f);
		ImGui::Checkbox("Use Albedo Map", (bool*)&material.useAlbedoMap);
		ImGui::Checkbox("Use Normal Map", (bool*)&material.useNormalMap);
		ImGui::Checkbox("Use Roughness Map", (bool*)&material.useRoughnessMap);
		ImGui::Checkbox("Use Metal Map", (bool*)&material.useMetalMap);
		ImGui::PopID();
	}

}