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
	class mvPBRMaterialCBuf : public mvBuffer
	{

	public:

		struct mvMaterialBuffer
		{
			glm::vec3 albedo = { 0.45f, 0.45f, 0.85f };
			float metalness = 0.5f;
			//-------------------------- ( 16 bytes )

			float roughness = 0.5f;
			float radiance = 1.0f;
			float fresnel = 0.04f;
			int useAlbedoMap = false;
			//-------------------------- ( 16 bytes )

			int useNormalMap = false;
			int useRoughnessMap = false;
			int useMetalMap = false;
			char _pad0[4];
			//-------------------------- ( 16 bytes )

			//-------------------------- ( 3 * 16 = 48 bytes )
		};

	public:

		mvPBRMaterialCBuf(mvGraphics& graphics, UINT slot);

		void bind(mvGraphics& graphics) override;

	public:

		mvMaterialBuffer material;

	private:

		std::unique_ptr<mvPixelConstantBuffer> m_buf;
	
	};

}