#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

namespace Marvel {

	class mvGraphics;

	class mvPipeline
	{

	public:

		void init(mvGraphics& graphics);

		VkPipeline       getPipeline() { return m_graphicsPipeline; }
		VkPipelineLayout getLayout() { return m_pipelineLayout; }

	private:

		VkPipelineLayout m_pipelineLayout;
		VkPipeline       m_graphicsPipeline;

	};

}