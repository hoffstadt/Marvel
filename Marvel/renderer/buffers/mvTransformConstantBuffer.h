#pragma once
#include <memory>
#include "mvBuffer.h"
#include "mvConstantBuffer.h"
#include "mvMath.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvDrawable;
	class mvGraphics;

	//-----------------------------------------------------------------------------
	// mvTransformUniform
	//-----------------------------------------------------------------------------
	class mvTransformConstantBuffer : public mvBuffer
	{

		struct Transforms
		{
			glm::mat4 model = glm::identity<glm::mat4>();
			glm::mat4 modelView = glm::identity<glm::mat4>();
			glm::mat4 modelViewProjection = glm::identity<glm::mat4>();
		};

	public:

		mvTransformConstantBuffer(mvGraphics& graphics);

		void bind(mvGraphics& graphics) override;

	private:

		std::unique_ptr<mvVertexConstantBuffer> m_buf;
		Transforms m_transforms;

	};

}