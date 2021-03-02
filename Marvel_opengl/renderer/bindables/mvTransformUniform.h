#pragma once
#include <memory>
#include "mvBindable.h"
#include "mvUniform.h"
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp> 

namespace Marvel {

	class mvDrawable;
	class mvGraphics;

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

		Transforms                 m_transforms;
		mvUniformMatrix<glm::mat4> m_model;
		mvUniformMatrix<glm::mat4> m_modelView;
		mvUniformMatrix<glm::mat4> m_modelViewProj;

	};

}