#pragma once
#include <memory>
#include "mvBindable.h"
#include "mvConstantBuffer.h"

namespace Marvel {

	class mvGraphics;

	class mvMaterial : public mvBindable
	{

	public:

		mvMaterial(mvGraphics& graphics, glm::vec3 materialColor = { 1.0f, 1.0f, 1.0f });

		void bind(mvGraphics& graphics) override;


	public:

		std::unique_ptr<mvPixelConstantBuffer> m_buf;
		std::unique_ptr<mvBuffer>              m_bufferRaw;

	};

}