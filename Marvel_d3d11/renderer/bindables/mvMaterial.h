#pragma once
#include <memory>
#include "mvBindable.h"
#include "mvConstantBuffer.h"

namespace Marvel {

	class mvGraphics;

	class mvMaterial : public mvBindable
	{

	public:

		struct Material
		{
			alignas(16) glm::vec3 materialColor;
			alignas(16) glm::vec3 specularColor;
			float specularWeight;
			float specularGloss;
		};

		mvMaterial(mvGraphics& graphics, glm::vec3 materialColor = { 1.0f, 1.0f, 1.0f });

		void bind(mvGraphics& graphics) override;


	public:

		Material m_cbData;
		mvGlobalConstantBuffer<Material> m_buf;

	};

}