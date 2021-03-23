#include "mvRenderGraph.h"
#include <vector>
#include <assert.h>
#include "mvPass.h"
#include "mvLambertianPass.h"
#include "mvSkyboxPass.h"
#include "mvGraphics.h"
#include "mvCommonBindables.h"

namespace Marvel {


	mvRenderGraph::mvRenderGraph(mvGraphics& graphics)
	{
		m_passes.push_back(std::make_shared<mvLambertianPass>(graphics));
		m_passes.push_back(std::make_shared<mvSkyboxPass>(graphics));
	}

	void mvRenderGraph::addJob(mvJob job, size_t target)
	{
		m_passes[target]->addJob(job);
	}

	void mvRenderGraph::execute(mvGraphics& graphics) const
	{
		for (auto& pass : m_passes)
			pass->execute(graphics);
	}

	void mvRenderGraph::reset()
	{
		for (auto& pass : m_passes)
			pass->reset();
	}

	mvPass* mvRenderGraph::getPass(const std::string& name)
	{
		for (auto& pass : m_passes)
		{
			if (pass->getName() == name)
				return pass.get();
		}

		assert(false);
	}
}
