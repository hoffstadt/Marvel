#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

namespace Marvel {

	class mvGraphics;

	class mvRenderPass
	{

	public:

		void init(mvGraphics& graphics);

		VkRenderPass getRenderPass() { return m_renderPass; }

	private:

		VkRenderPass m_renderPass;

	};

}