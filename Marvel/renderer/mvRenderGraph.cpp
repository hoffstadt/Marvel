#include "mvRenderGraph.h"
#include <vector>
#include "mvPass.h"
#include "mvGraphics.h"
#include "mvCommonBindables.h"

namespace Marvel {


	mvRenderGraph::mvRenderGraph(mvGraphics& graphics)
	{
		// regular pass
		mvPass lambertian;
		lambertian.addBindable(std::make_shared<mvStencil>(graphics, mvStencil::Mode::Off));

		m_passes.push_back(lambertian);

		// skybox pass
		mvPass skypass;
		skypass.addBindable(std::make_shared<mvCubeTexture>(graphics, "C:\\dev\\Marvel\\Resources\\SkyBox"));
		skypass.addBindable(std::make_shared<mvStencil>(graphics, mvStencil::Mode::DepthFirst));
		skypass.addBindable(std::make_shared<mvSampler>(graphics, mvSampler::Type::Bilinear));
		skypass.addBindable(std::make_shared<mvRasterizer>(graphics, true));
		skypass.addBindable(std::make_shared<mvPixelShader>(graphics, "../../Marvel/shaders/Skybox_PS.hlsl"));
		auto vshader = std::make_shared<mvVertexShader>(graphics, "../../Marvel/shaders/Skybox_VS.hlsl");
		skypass.addBindable(vshader);
		skypass.addBindable(std::make_shared<mvTopology>(graphics, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		skypass.addBindable(std::make_shared<mvSkyBoxTransformConstantBuffer>(graphics));

		// create vertex layout
		mvVertexLayout vl;
		vl.append(ElementType::Position3D);

		constexpr float side = 1.0f / 2.0f;

		// create vertex buffer
		m_vertexBuffer = std::make_shared<mvVertexBuffer>(graphics, std::vector<float>{
			-side,-side,-side,
			 side,-side,-side,
			-side, side,-side,
			 side, side,-side,
			-side,-side, side,
			 side,-side, side,
			-side, side, side,
			 side, side, side
		}, vl);

		// create index buffer
		m_indexBuffer = std::make_shared<mvIndexBuffer>(graphics,
			std::vector<unsigned short>{
				0, 2, 1, 2, 3, 1,
				1, 3, 5, 3, 7, 5,
				2, 6, 3, 3, 6, 7,
				4, 5, 7, 4, 7, 6,
				0, 4, 2, 2, 4, 6,
				0, 1, 4, 1, 5, 4
		});

		skypass.addBindable(std::make_shared<mvInputLayout>(graphics, vl,
			static_cast<mvVertexShader*>(vshader.get())));

		m_passes.push_back(skypass);
	}

	void mvRenderGraph::addJob(mvJob job, size_t target)
	{
		m_passes[target].addJob(job);
	}

	void mvRenderGraph::execute(mvGraphics& graphics) const
	{
		for (auto& bind : m_passes[0].m_bindables)
			bind->bind(graphics);
		m_passes[0].execute(graphics);

			m_vertexBuffer->bind(graphics);
			m_indexBuffer->bind(graphics);

			//m_passes[1].execute(graphics);
			for (auto& bind : m_passes[1].m_bindables)
				bind->bind(graphics);
			graphics.drawIndexed(m_indexBuffer->getCount());
	}

	void mvRenderGraph::reset()
	{
		for (auto& pass : m_passes)
			pass.reset();
	}

}
