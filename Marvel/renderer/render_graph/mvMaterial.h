#pragma once
#include <memory>
#include <filesystem>
#include <vector>
#include "mvBindable.h"
#include "mvConstantBuffer.h"
#include "mvTechnique.h"
#include "mvVertexLayout.h"

//-----------------------------------------------------------------------------
// forward declarations
//-----------------------------------------------------------------------------
struct aiMaterial;

namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvGraphics;

	//-----------------------------------------------------------------------------
	// mvMaterial
	//-----------------------------------------------------------------------------
	class mvMaterial
	{

	public:

		mvMaterial(mvGraphics& graphics, const aiMaterial& material, const std::string& path);

		std::vector<mvTechnique> getTechniques() const;
		const mvVertexLayout&    getLayout() const;

	private:

		std::vector<mvTechnique> m_techniques;
		mvVertexLayout           m_layout;

	};

}