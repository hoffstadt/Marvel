#pragma once
#include <d3d11.h>
#include "mvBindable.h"
#include "mvComPtr.h"

//-----------------------------------------------------------------------------
// forward declarations
//-----------------------------------------------------------------------------
struct ID3D11DepthStencilView;

namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvGraphics;

	//-----------------------------------------------------------------------------
	// mvDepthStencil
	//-----------------------------------------------------------------------------
	class mvDepthStencil : public mvBindable
	{

	public:

		mvDepthStencil(mvGraphics& graphics, int width, int height);

		void bind(mvGraphics& graphics) override {}

		void                    clear              (mvGraphics& graphics);
		ID3D11DepthStencilView* getDepthStencilView();
		void reset();

	private:

		mvComPtr<ID3D11DepthStencilView> m_DSV;

	};

}