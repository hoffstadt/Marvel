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

		static std::shared_ptr<mvInputLayout> Request(mvGraphics& graphics, const mvVertexLayout& vertexLayout, const mvVertexShader& vertexShader);
		static std::string                    GenerateUniqueIdentifier(const mvVertexLayout& vertexLayout, const mvVertexShader& vertexShader);

	public:

		mvInputLayout(mvGraphics& graphics, const mvVertexLayout& vertexLayout, const mvVertexShader& vertexShader);

		void bind(mvGraphics& graphics) override;

	private:

		mvVertexLayout                      m_layout;
		Marvel::mvComPtr<ID3D11InputLayout> m_inputLayout;
		std::string                         m_shaderId;

	};

}