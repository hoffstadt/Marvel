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

		struct mvMaterialBuffer
		{
			glm::vec3 materialColor = {0.45f, 0.45f, 0.85f};
			float padding1 = 0.0f;
			//-------------------------- ( 16 bytes )

			glm::vec3 specularColor = {0.18f, 0.18f, 0.18f};
			float specularWeight = 1.0f;
			//-------------------------- ( 16 bytes )

			float specularGloss = 8.0f;
			float normalMapWeight = 1.0f;
			int useTextureMap = false;
			int useNormalMap = false;
			//-------------------------- ( 16 bytes )

			int useSpecularMap = false;
			int useGlossAlpha = false;

			char padding6[8];
			//-------------------------- ( 16 bytes )
			//-------------------------- ( 4 * 16 = 64 bytes )
		};

	public:

		mvMaterial(mvGraphics& graphics, const aiMaterial& material, const std::string& path);

		std::vector<mvTechnique> getTechniques() const;
		const mvVertexLayout&    getLayout() const;

	private:

		mvMaterialBuffer         m_materialBuffer{};
		std::vector<mvTechnique> m_techniques;
		mvVertexLayout           m_layout;

	};

}