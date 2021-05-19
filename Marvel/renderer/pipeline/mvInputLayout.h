#pragma once
#include "mvGraphics.h"
#include "mvVertexLayout.h"
#include "mvVertexShader.h"
#include <vector>

namespace Marvel {

	class mvInputLayout
	{

	public:

		static mvInputLayout* Request(mvGraphics& graphics, const mvVertexLayout& vertexLayout, const mvVertexShader& vertexShader);
		static std::string    GenerateUniqueIdentifier(const mvVertexLayout& vertexLayout, std::string shaderId);

	public:

		mvInputLayout(mvGraphics& graphics, const mvVertexLayout& vertexLayout, const mvVertexShader& vertexShader);

		void set(mvGraphics& graphics);
		std::string getUniqueIdentifier() const;

	private:

		mvVertexLayout                      m_layout;
		Marvel::mvComPtr<ID3D11InputLayout> m_inputLayout;
		std::string                         m_shaderId;

	};

}