#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <optional>

namespace Marvel {

	class mvGraphics;

	struct QueueFamilyIndices
	{

		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete()
		{
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	class mvSurface
	{

	public:

		void init   (mvGraphics& graphics);
		void cleanup(mvGraphics& graphics);

		QueueFamilyIndices      findQueueFamilies    (VkPhysicalDevice device);
		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

		VkSurfaceKHR getSurface() { return m_surface; }

	private:

		VkSurfaceKHR m_surface;

	};

}