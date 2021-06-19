#pragma once
#include <memory>
#include <filesystem>
#include <vector>
#include "mvBindable.h"
#include "mvMaterial.h"
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
	// mvPhongMaterial
	//-----------------------------------------------------------------------------
	class mvPhongMaterial : public mvMaterial
	{

	public:

		mvPhongMaterial(mvGraphics& graphics, const aiMaterial& material, const std::string& path);

		void showControls() override { m_material->showControls(); }

	private:

		std::shared_ptr<mvPhongMaterialCBuf> m_material = nullptr;


	};

}