#pragma once
#include <memory>
#include "mvBindable.h"
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
	class mvTransformConstantBuffer : public mvBindable
	{

	public:

		struct Transforms
		{
			glm::mat4 model = glm::identity<glm::mat4>();
			glm::mat4 modelView = glm::identity<glm::mat4>();
			glm::mat4 modelViewProj = glm::identity<glm::mat4>();
		};

		mvTransformConstantBuffer(mvGraphics& graphics);
		~mvTransformConstantBuffer();

		void bind(mvGraphics& graphics) override;

		Transforms getTransforms(mvGraphics& graphics);

	private:

		mvVertexConstantBuffer<Transforms>* m_buf;

	};

}