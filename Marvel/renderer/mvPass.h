#pragma once
#include <vector>
#include "mvGraphics.h"
#include "mvJob.h"

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

	private:

		std::vector<mvJob> m_jobs;
	};
}