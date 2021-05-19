#pragma once

#include <d3d11_1.h>
#include <vector>
#include <string>
#include <memory>
#include "mvComPtr.h"


namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvGraphics;

	//-----------------------------------------------------------------------------
	// mvDepthStencilState
	//-----------------------------------------------------------------------------
	class mvDepthStencilState
	{

	public:

		enum class Mode
		{
			Off,
			Write,
			Mask,
			DepthOff,
			DepthReversed,
			DepthFirst, // for skybox render
		};

	public:

		static mvDepthStencilState* Request(mvGraphics& graphics, Mode mode);
		static std::string GenerateUniqueIdentifier(Mode mode);

	public:

		void set(mvGraphics& graphics);

	private:

		mvDepthStencilState(mvGraphics& graphics, Mode mode);

		std::string getUniqueIdentifier() const;

	private:

		mvComPtr<ID3D11DepthStencilState> m_state;
		Mode                              m_mode;
	};

}