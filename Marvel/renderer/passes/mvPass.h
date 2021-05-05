#pragma once

#include <vector>
#include <memory>
#include <string>
#include "mvJob.h"
#include "mvBindable.h"

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

		virtual void execute(mvGraphics& graphics) const;

		void               addJob     (mvJob job);
		void               reset      ();
		void               addBindable(std::shared_ptr<mvBindable> bindable);
		const std::string& getName    () const;

	protected:

		std::vector<mvJob>                       m_jobs;
		std::vector<std::shared_ptr<mvBindable>> m_bindables;
		std::string                              m_name;
		std::shared_ptr<mvDepthStencil>          m_depthStencil;
		std::shared_ptr<mvRenderTarget>          m_renderTarget;
	};
}