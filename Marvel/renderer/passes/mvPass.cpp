#include "mvPass.h"
#include "mvGraphics.h"

namespace Marvel {

	mvPass::mvPass(const std::string& name)
	{
		m_name = name;
	}

	void mvPass::addJob(mvJob job)
	{
		m_jobs.push_back(job);
	}

	void mvPass::execute(mvGraphics& graphics) const
	{
		for (const auto& j : m_jobs)
			j.execute(graphics);
	}

	void mvPass::reset()
	{
		m_jobs.clear();
	}

	void mvPass::addBindable(std::shared_ptr<mvBindable> bindable)
	{
		m_bindables.push_back(bindable);
	}

	const std::string& mvPass::getName() const
	{
		return m_name;
	}
}