#pragma once

#include <vector>
#include <memory>
#include <string>
#include "mvJob.h"
#include "mvBindable.h"
#include "mvBuffer.h"

namespace Marvel{

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvGraphics;
	class mvDepthStencil;
	class mvRenderTarget;

	//-----------------------------------------------------------------------------
	// mvPass
	// 	   * collection of jobs
	// 	   * pass-wide bindables
	//-----------------------------------------------------------------------------
	class mvPass
	{

		friend class mvRenderGraph;

	public:

		mvPass(const std::string& name);
		virtual ~mvPass() = default;

		virtual void execute(mvGraphics& graphics) const = 0;

		void               addJob(mvJob job);
		void               clearJobs();
		void               releaseBuffers();
		void               addBindable(std::shared_ptr<mvBindable> bindable);
		void               addBuffer(std::shared_ptr<mvBuffer> buffer);
		const std::string& getName() const;

		std::shared_ptr<mvRenderTarget> getRenderTarget();
		std::shared_ptr<mvDepthStencil> getDepthStencil();

		void linkRenderTarget(mvPass& pass);
		void linkRenderTarget(mvRenderGraph& graph);
		void linkDepthStencil(mvPass& pass);
		void linkDepthStencil(mvRenderGraph& graph);

	protected:

		std::shared_ptr<mvDepthStencil>          m_depthStencil;
		std::shared_ptr<mvRenderTarget>          m_renderTarget;
		std::vector<std::shared_ptr<mvBindable>> m_bindables;
		std::vector<std::shared_ptr<mvBuffer>>   m_buffers;
		std::vector<mvJob>                       m_jobs;
		std::string                              m_name;
	};
}