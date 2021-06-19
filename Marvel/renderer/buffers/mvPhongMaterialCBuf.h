#pragma once
#include <memory>
#include "mvBuffer.h"
#include "mvConstantBuffer.h"
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
	class mvPhongMaterialCBuf : public mvBuffer
	{

	public:

		struct mvMaterialBuffer
		{
			glm::vec4 materialColor = { 0.45f, 0.45f, 0.85f, 1.0f };
			//-------------------------- ( 16 bytes )

			glm::vec3 specularColor = { 0.18f, 0.18f, 0.18f };
			float specularGloss = 8.0f;
			//-------------------------- ( 16 bytes )

			float normalMapWeight = 1.0f;
			int useTextureMap = false;
			int useNormalMap = false;
			int useSpecularMap = false;
			//-------------------------- ( 16 bytes )

			int useGlossAlpha = false;
			int hasAlpha = false;
			char _pad1[8];
			//-------------------------- ( 16 bytes )
			//-------------------------- ( 4 * 16 = 64 bytes )
		};

	public:

		mvPhongMaterialCBuf(mvGraphics& graphics, UINT slot);

		void bind(mvGraphics& graphics) override;

		void showControls();

	public:

		mvMaterialBuffer material;

	private:

		std::unique_ptr<mvPixelConstantBuffer> m_buf;
	
	};

}