#include "mvTransformUniform.h"
#include "drawables/mvDrawable.h"
#include "mvGraphics.h"

namespace Marvel {

	mvTransformUniform::mvTransformUniform(mvGraphics& graphics)
		:
		m_model(mvUniformMatrix<glm::mat4>(graphics, m_transforms.model, 20)),
		m_modelView(mvUniformMatrix<glm::mat4>(graphics, m_transforms.modelView, 40)),
		m_modelViewProj(mvUniformMatrix<glm::mat4>(graphics, m_transforms.modelViewProj, 60))
	{

	}


	void mvTransformUniform::bind(mvGraphics& graphics)
	{
		auto transforms = getTransforms(graphics);
		m_model.update(transforms.model);
		m_modelView.update(transforms.modelView);
		m_modelViewProj.update(transforms.modelViewProj);

		m_model.bind(graphics);
		m_modelView.bind(graphics);
		m_modelViewProj.bind(graphics);
	}

	mvTransformUniform::Transforms mvTransformUniform::getTransforms(mvGraphics& graphics)
	{
		auto model = m_parent->getTransform();
		auto modelView = graphics.getCamera() * model;
		auto modelViewProj = graphics.getProjection() * modelView;
		return{ model, modelView, modelViewProj };
	}

}