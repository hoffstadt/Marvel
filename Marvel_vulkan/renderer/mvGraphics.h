#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <optional>
#include <set>

namespace Marvel {

	class mvGraphics
	{

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

	public:

		mvGraphics(GLFWwindow* window);

		void drawFrame();
		void cleanup();

	private:

		void createInstance();
		void setupDebugMessenger();
		void createSurface();
		void pickPhysicalDevice();
		void createLogicalDevice();
		void createSwapChain();
		void createImageViews();
		void createRenderPass();
		void createGraphicsPipeline();
		void createFramebuffers();
		void createCommandPool();
		void createVertexBuffer();
		void createCommandBuffers();
		void createSyncObjects();

		// cleanup
		void cleanupSwapChain();

		// helpers
		bool checkValidationLayerSupport();
		bool checkDeviceExtensionSupport(VkPhysicalDevice device);
		bool isDeviceSuitable(VkPhysicalDevice device);
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	private:

		GLFWwindow*                        m_window;
		VkInstance                         m_instance;
		VkDebugUtilsMessengerCreateInfoEXT m_debugCreateInfo = {};
		VkDebugUtilsMessengerEXT           m_debugMessenger;
		VkPhysicalDevice                   m_physicalDevice = VK_NULL_HANDLE;
		VkSurfaceKHR                       m_surface;
		VkDevice                           m_device;
		VkQueue                            m_graphicsQueue;
		VkQueue                            m_presentQueue;
		VkSwapchainKHR                     m_swapChain;
		std::vector<VkImage>               m_swapChainImages;
		std::vector<VkImageView>           m_swapChainImageViews;
		VkFormat                           m_swapChainImageFormat;
		VkExtent2D                         m_swapChainExtent;
		VkPipelineLayout                   m_pipelineLayout;
		VkPipeline                         m_graphicsPipeline;
		VkRenderPass                       m_renderPass;
		std::vector<VkFramebuffer>         m_swapChainFramebuffers;
		VkCommandPool                      m_commandPool;
		std::vector<VkCommandBuffer>       m_commandBuffers;
		std::vector<VkSemaphore>           m_imageAvailableSemaphores;
		std::vector<VkSemaphore>           m_renderFinishedSemaphores;
		std::vector<VkFence>               m_inFlightFences;
		std::vector<VkFence>               m_imagesInFlight;
		VkBuffer                           m_vertexBuffer;
		VkDeviceMemory                     m_vertexBufferMemory;

		const std::vector<const char*> m_validationLayers = {"VK_LAYER_KHRONOS_validation"};
		const std::vector<const char*> m_deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
		const bool                     m_enableValidationLayers = true;
		const int                      MAX_FRAMES_IN_FLIGHT = 2;

	};

}