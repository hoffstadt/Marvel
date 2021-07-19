#include "mvStep.h"
#include "mvDrawable.h"
#include "mvPass.h"
#include "mvJob.h"
#include "mvRenderGraph.h"

namespace Marvel {

	mvStep::mvStep(const std::string& targetPass)
	{
		m_targetPass = targetPass;
	}

	void mvStep::link(mvRenderGraph& graph)
	{
		m_pass = graph.getPass(m_targetPass);
	}

	void mvStep::addBindable(std::shared_ptr<mvBindable> bindable)
	{
		m_bindables.push_back(std::move(bindable));
	}

	void mvStep::addBuffer(std::shared_ptr<mvBuffer> buffer)
	{
		m_buffers.push_back(std::move(buffer));
	}

	void mvStep::bind(mvGraphics& graphics, const mvDrawable* parent) const
	{
		m_pipeline->set(graphics);

		for (auto& buffer : m_buffers)
		{
			buffer->m_parent = parent;
			buffer->bind(graphics);
		}

		for (auto& bindable : m_bindables)
			bindable->bind(graphics);
	}

	void mvStep::submit(const mvDrawable& drawable) const
	{
		assert(m_pass != nullptr);
		m_pass->addJob(mvJob(this, &drawable));
	}

	void mvStep::registerPipeline(mvGraphics& graphics, mvPipelineInfo& info)
	{
		m_pipeline = mvPipeline::Request(graphics, info);
	}

}