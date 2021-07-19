#include "mvPass.h"
#include "mvRenderGraph.h"
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


	void mvPass::clearJobs()
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

	std::shared_ptr<mvRenderTarget> mvPass::getRenderTarget()
	{
		return m_renderTarget;
	}

	std::shared_ptr<mvDepthStencil> mvPass::getDepthStencil()
	{
		return m_depthStencil;
	}

	void mvPass::linkRenderTarget(mvPass& pass)
	{
		m_renderTarget = pass.m_renderTarget;
	}

	void mvPass::linkDepthStencil(mvPass& pass)
	{
		m_depthStencil = pass.m_depthStencil;
	}

	void mvPass::linkRenderTarget(mvRenderGraph& graph)
	{
		m_renderTarget = graph.getMasterRenderTarget();
	}

	void mvPass::linkDepthStencil(mvRenderGraph& graph)
	{
		m_depthStencil = graph.getMasterDepthStencil();
	}
}