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

		enum class Usage{ DepthStencil, ShadowDepth};

		mvDepthStencil(mvGraphics& graphics, int width, int height, bool shaderBinding = false, Usage usage = Usage::DepthStencil);

		void bind(mvGraphics& graphics) override {}

		void clear(mvGraphics& graphics);
		ID3D11DepthStencilView* getDepthStencilView();
		void reset();

	protected:

		mvComPtr<ID3D11DepthStencilView> m_DSV;

	};

	//-----------------------------------------------------------------------------
	// mvInputDepthStencil
	//-----------------------------------------------------------------------------
	class mvInputDepthStencil : public mvDepthStencil
	{

	public:

		mvInputDepthStencil(mvGraphics& graphics, int width, int height, UINT slot, Usage usage = Usage::DepthStencil);
		mvInputDepthStencil(mvGraphics& graphics, UINT slot, Usage usage = Usage::DepthStencil);

		void bind(mvGraphics& graphics) override;

	private:

		UINT m_slot;
		mvComPtr<ID3D11ShaderResourceView> m_shaderResourceView;

	};

	//-----------------------------------------------------------------------------
	// mvOutputDepthStencil
	//-----------------------------------------------------------------------------
	class mvOutputDepthStencil : public mvDepthStencil
	{

	public:

		mvOutputDepthStencil(mvGraphics& graphics, int width, int height);

		void bind(mvGraphics& graphics) override {}

	};

}