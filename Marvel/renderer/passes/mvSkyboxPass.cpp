#include "mvSkyboxPass.h"
#include "mvGraphics.h"
#include "mvCommonBindables.h"

namespace Marvel {

	mvSkyboxPass::mvSkyboxPass(mvGraphics& graphics, const char* skybox)
		:
		mvPass("Skybox")
	{
		addBindable(std::make_shared<mvCubeTexture>(graphics, skybox));
		addBindable(std::make_shared<mvStencil>(graphics, mvStencil::Mode::DepthFirst));
		addBindable(std::make_shared<mvSampler>(graphics, mvSampler::Type::Bilinear));
		addBindable(std::make_shared<mvRasterizer>(graphics, true));
		addBindable(std::make_shared<mvPixelShader>(graphics, std::string(graphics.getShaderRoot() + "Skybox_PS.hlsl").c_str()));
		auto vshader = std::make_shared<mvVertexShader>(graphics, std::string(graphics.getShaderRoot() + "Skybox_VS.hlsl").c_str());
		addBindable(vshader);
		addBindable(std::make_shared<mvTopology>(graphics, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		addBindable(std::make_shared<mvSkyBoxTransformConstantBuffer>(graphics));

		// create vertex layout
		mvVertexLayout vl;
		vl.append(ElementType::Position3D);

		constexpr float side = 1.0f / 2.0f;

		// create vertex buffer
		m_vertexBuffer = std::make_shared<mvVertexBuffer>(graphics, std::vector<float>{
			-side, -side, -side,
				side, -side, -side,
				-side, side, -side,
				side, side, -side,
				-side, -side, side,
				side, -side, side,
				-side, side, side,
				side, side, side
		}, vl);

		// create index buffer
		m_indexBuffer = std::make_shared<mvIndexBuffer>(graphics,
			std::vector<unsigned int>{
			0, 2, 1, 2, 3, 1,
				1, 3, 5, 3, 7, 5,
				2, 6, 3, 3, 6, 7,
				4, 5, 7, 4, 7, 6,
				0, 4, 2, 2, 4, 6,
				0, 1, 4, 1, 5, 4
		});

		addBindable(std::make_shared<mvInputLayout>(graphics, vl,
			static_cast<mvVertexShader*>(vshader.get())));
	}

	void mvSkyboxPass::execute(mvGraphics& graphics) const
	{
		m_vertexBuffer->bind(graphics);
		m_indexBuffer->bind(graphics);

		//m_passes[1].execute(graphics);
		for (auto& bind : m_bindables)
			bind->bind(graphics);
		graphics.drawIndexed(m_indexBuffer->getCount());
	}

}