#pragma once
#include <array>
#include <d3d11.h>
#include "mvBindable.h"
#include "mvComPtr.h"

namespace Marvel{

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvGraphics;

	//-----------------------------------------------------------------------------
	// mvRasterizer
	//-----------------------------------------------------------------------------
	class mvRasterizer : public mvBindable
	{

	public:

		static std::shared_ptr<mvRasterizer> Request(mvGraphics& graphics, bool twoSided);
		static std::string                   GenerateUniqueIdentifier(bool twoSided);

	public:

		mvRasterizer(mvGraphics& graphics, bool twoSided);

		void bind(mvGraphics& graphics) override;
		std::string getUniqueIdentifier() const override;

	protected:

		mvComPtr<ID3D11RasterizerState> m_rasterizer;
		bool                            m_twoSided;
	};
}
