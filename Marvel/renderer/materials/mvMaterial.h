#pragma once
#include <memory>
#include <filesystem>
#include <vector>
#include "mvBindable.h"
#include "mvConstantBuffer.h"
#include "mvTechnique.h"
#include "mvVertexLayout.h"
#include "mvPhongMaterialCBuf.h"
#include "mvPBRMaterialCBuf.h"

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

		std::vector<mvTechnique> getTechniques() const;
		const mvVertexLayout&    getLayout() const;
		virtual void showControls() {}

	protected:

		std::vector<mvTechnique> m_techniques;
		mvVertexLayout           m_layout;

	};

}