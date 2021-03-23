#pragma once

#include <array>
#include "mvPass.h"
#include "mvGraphics.h"

namespace Marvel {

	class mvRenderGraph
	{

	public:

		void addJob(mvJob job, size_t target)
		{
			m_passes[target].addJob(job);
		}

		void execute(mvGraphics& graphics) const
		{
			m_passes[0].execute(graphics);
		}

		void reset()
		{
			for (auto& pass : m_passes)
				pass.reset();
		}

	public:

		std::array<mvPass, 1> m_passes;

	};

}
