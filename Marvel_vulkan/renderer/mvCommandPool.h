#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

namespace Marvel {

	class mvGraphics;

	class mvCommandPool
	{

	public:

		void init   (mvGraphics& graphics);
		void cleanup(mvGraphics& graphics);

		void createCommandPool   (mvGraphics& graphics);
		void createCommandBuffers(mvGraphics& graphics, VkBuffer vertexBuffer);

		VkCommandPool                 getPool          () { return m_commandPool; }
		std::vector<VkCommandBuffer>& getCommandBuffers() { return m_commandBuffers; }

	private:

		VkCommandPool                m_commandPool;
		std::vector<VkCommandBuffer> m_commandBuffers;

	};

}