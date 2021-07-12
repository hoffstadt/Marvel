#pragma once

#include <vector>
#include <memory>
#include "mvConstantBuffer.h"
#include "mvPass.h"
#include "mvVertexBuffer.h"
#include "mvIndexBuffer.h"
#include "mvBaseRenderGraph.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvGraphics;
	class mvCamera;

	//-----------------------------------------------------------------------------
	// mvRenderGraph
	//-----------------------------------------------------------------------------
	class mvEmptyGraph : public mvBaseRenderGraph
	{

	public:

		mvEmptyGraph(mvGraphics& graphics);

		void bindMainCamera(mvCamera& camera) override;

	};

}
