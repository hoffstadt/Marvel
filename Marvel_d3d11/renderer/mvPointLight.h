#pragma once
#include "mvGraphics.h"
#include "mvConstantBuffer.h"
#include "mvSolidSphere.h"

namespace Marvel {

	class mvPointLight
	{

	public:

		mvPointLight(mvGraphics& graphics, glm::vec3 pos = { 0.0f,0.0f,0.5f });

		void show_imgui_windows();
		void bind(mvGraphics& graphics, glm::mat4 view);

		mvSolidSphere* getSphere();

	private:

		struct mvPointLightCBuf
		{
			alignas(16) glm::vec3 pos;
			alignas(16) glm::vec3 ambient;
			alignas(16) glm::vec3 diffuseColor;
			float diffuseIntensity;
			float attConst;
			float attLin;
			float attQuad;
		};

	private:

		mvSolidSphere                            m_sphere;
		mvPointLightCBuf                         m_cbData;
		mvGlobalConstantBuffer<mvPointLightCBuf> m_cbuf;

	};

}