#include "mvPass.h"
#include "mvGraphics.h"
#include "mvRenderTarget.h"
#include "mvDepthStencil.h"

namespace Marvel {

	mvPass::mvPass(const std::string& name)
	{
		m_name = name;
		m_bindables.reserve(16);
	}

	void mvPass::addJob(mvJob job)
	{
		m_jobs.push_back(job);
	}


	void mvPass::reset()
	{
		m_jobs.clear();
	}

	void mvPass::addBindable(std::shared_ptr<mvBindable> bindable)
	{
		m_bindables.push_back(std::move(bindable));
	}

	void mvPass::addBuffer(std::shared_ptr<mvBuffer> buffer)
	{
		m_buffers.push_back(std::move(buffer));
	}

	const std::string& mvPass::getName() const
	{
		return m_name;
	}

	void mvPass::releaseBuffers()
	{
		m_renderTarget->reset();
		m_renderTarget.reset();
		m_depthStencil->reset();
		m_depthStencil.reset();
	}

	bool mvPass::isLinked() const
	{

		return true;
	}
}