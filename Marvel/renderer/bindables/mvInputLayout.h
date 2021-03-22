#pragma once
#include "mvBindable.h"
#include "mvGraphics.h"
#include "mvVertexLayout.h"
#include "mvVertexShader.h"
#include <vector>

namespace Marvel {

	class mvInputLayout : public mvBindable
	{

	public:

		mvInputLayout(mvGraphics& graphics, mvVertexLayout vertexLayout, mvVertexShader* vertexShader);

		void bind(mvGraphics& graphics) override;

	private:

		mvVertexLayout                      m_layout;
		Marvel::mvComPtr<ID3D11InputLayout> m_inputLayout;

	};

}