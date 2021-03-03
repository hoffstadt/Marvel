#pragma once
#include <memory>
#include "mvBindable.h"
#include "mvUniform.h"
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
	class mvTransformUniform : public mvBindable
	{

	public:

		struct Transforms
		{
			glm::mat4 model = glm::identity<glm::mat4>();
			glm::mat4 modelView = glm::identity<glm::mat4>();
			glm::mat4 modelViewProj = glm::identity<glm::mat4>();
		};

		mvTransformUniform(mvGraphics& graphics);

		void bind(mvGraphics& graphics) override;

		Transforms getTransforms(mvGraphics& graphics);

	private:

		Transforms                  m_transforms;
		mvUniform4Matrix<glm::mat4> m_model;
		mvUniform4Matrix<glm::mat4> m_modelView;
		mvUniform4Matrix<glm::mat4> m_modelViewProj;

	};

}