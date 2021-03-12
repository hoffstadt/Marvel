#pragma once
#include <d3d11.h>
#include "mvBindable.h"

namespace Marvel {

	class mvGraphics;

	class mvNullGeometryShader : public mvBindable
	{

	public:

		mvNullGeometryShader(mvGraphics& graphics);

        void bind(mvGraphics& graphics) override;


	};

}