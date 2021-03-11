#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

namespace Marvel {

	class mvGraphics;

	class mvDevice
	{

	public:

		static bool IsDeviceSuitable(mvGraphics& graphics, VkPhysicalDevice device);
		static bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
		static uint32_t FindMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);


	public:

		void init   (mvGraphics& graphics);
		void cleanup(mvGraphics& graphics);

		VkQueue          getGraphicsQueue () { return m_graphicsQueue; }
		VkQueue          getPresentQueue  () { return m_presentQueue; }
		VkPhysicalDevice getPhysicalDevice() { return m_physicalDevice; }
		VkDevice         getLogicalDevice () { return m_device; }

	private:

		void pickPhysicalDevice (mvGraphics& graphics);
		void createLogicalDevice(mvGraphics& graphics);


	private:

		VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
		VkDevice         m_device;
		VkQueue          m_graphicsQueue;
		VkQueue          m_presentQueue;

		static const std::vector<const char*> s_deviceExtensions;

	};

}