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

		mvMaterial(mvGraphics& graphics, const aiMaterial& material, const std::string& path, bool PBR = false);

		std::vector<mvTechnique> getTechniques() const;
		const mvVertexLayout&    getLayout() const;
		mvPBRMaterialCBuf* getPBRMaterial() { return m_pbrMaterial.get();}
		mvPhongMaterialCBuf* getPhongMaterial() { return m_phongMaterial.get();}

	private:

		std::vector<mvTechnique> m_techniques;
		mvVertexLayout           m_layout;
		bool                     m_pbr = false;
		std::shared_ptr<mvPBRMaterialCBuf> m_pbrMaterial = nullptr;
		std::shared_ptr<mvPhongMaterialCBuf> m_phongMaterial = nullptr;

	};

}