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

		void               addJob     (mvJob job);
		void               reset      ();
		void               releaseBuffers();
		void               addBindable(std::shared_ptr<mvBindable> bindable);
		void               addBuffer(std::shared_ptr<mvBuffer> buffer);
		const std::string& getName    () const;
		bool               isLinked() const;

	public:

		std::vector<mvJob>                       m_jobs;
		std::vector<std::shared_ptr<mvBindable>> m_bindables;
		std::vector<std::shared_ptr<mvBuffer>>   m_buffers;
		std::string                              m_name;
		std::shared_ptr<mvDepthStencil>          m_depthStencil;
		std::shared_ptr<mvRenderTarget>          m_renderTarget;
	};
}