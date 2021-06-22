#include "mvMaterial.h"
#include "mvGraphics.h"
#include "mvCommonBindables.h"
#include "mvCommonBuffers.h"

namespace Marvel {

	std::vector<mvTechnique> mvMaterial::getTechniques() const
	{
		return m_techniques;
	}

	const mvVertexLayout& mvMaterial::getLayout() const
	{
		return m_layout;
	}

}