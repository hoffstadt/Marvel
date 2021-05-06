#pragma once
#include <d3d11.h>
#include "mvBindable.h"
#include "mvComPtr.h"
#include "mvBufferResource.h"

//-----------------------------------------------------------------------------
// forward declarations
//-----------------------------------------------------------------------------
struct ID3D11DepthStencilView;

namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvGraphics;
	class mvRenderTarget;

	//-----------------------------------------------------------------------------
	// mvDepthStencil
	//-----------------------------------------------------------------------------
	class mvDepthStencil : public mvBindable, public mvBufferResource
	{

	public:

		enum class Usage{ DepthStencil, ShadowDepth};

		mvDepthStencil(mvGraphics& graphics, int width, int height, bool shaderBinding = false, Usage usage = Usage::DepthStencil);

		// bindable interface
		void bind(mvGraphics& graphics) override {}

		// buffer resource interface
		void bindAsBuffer(mvGraphics& graphics) override;
		void bindAsBuffer(mvGraphics& graphics, mvBufferResource* renderTarget) override;
		void bindAsBuffer(mvGraphics& graphics, mvRenderTarget* renderTarget);
		void clear(mvGraphics& graphics) override;

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
		mvOutputDepthStencil(mvGraphics& graphics);

		void bind(mvGraphics& graphics) override {}

	};

}