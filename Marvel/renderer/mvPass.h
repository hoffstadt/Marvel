#pragma once
#include <vector>
#include "mvGraphics.h"
#include "mvJob.h"
#include "mvBindable.h"

namespace Marvel{

	class mvPass
	{

	public:

		void addJob(mvJob job)
		{
			m_jobs.push_back(job);
		}

		void execute(mvGraphics& graphics) const
		{
			for (const auto& j : m_jobs)
				j.execute(graphics);
		}

		void reset()
		{
			m_jobs.clear();
		}

		void addBindable(std::shared_ptr<mvBindable> bindable)
		{
			m_bindables.push_back(bindable);
		}

	public:

		std::vector<mvJob>                       m_jobs;
		std::vector<std::shared_ptr<mvBindable>> m_bindables;
	};
}