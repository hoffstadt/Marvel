#include "mvMaterial.h"
#include "mvGraphics.h"
#include "mvCommonBindables.h"
#include "mvCommonBuffers.h"

namespace Marvel {

	std::vector<mvStep> mvMaterial::getSteps() const
	{
		return m_steps;
	}

	const mvVertexLayout& mvMaterial::getLayout() const
	{
		return m_layout;
	}

}