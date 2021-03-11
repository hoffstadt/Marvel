#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <array>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <set>

#include "mvDevice.h"
#include "mvSwapChain.h"
#include "mvRenderPass.h"
#include "mvPipeline.h"
#include "mvCommandPool.h"
#include "mvSurface.h"
#include "mvVertexBuffer.h"

namespace Marvel {

	class mvGraphics
	{

	public:

		static bool                            CheckValidationLayerSupport();
		static const std::vector<const char*>& GetValidationLayers        ();
		static bool                            UseValidationLayers        () { return true; }
		static constexpr int                   GetMaxFramesInFlight       () { return 2; }

	public:

		mvGraphics(GLFWwindow* window);

		void drawFrame();
		void cleanup();

		GLFWwindow*    getWindow     () { return m_window; }
		VkInstance     getInstance   () { return m_instance; }
		mvDevice&      getDevice     () { return m_device; }
		mvSurface      getSurface    () { return m_surface; }
		mvSwapChain&   getSwapChain  () { return m_swapChain; }
		mvRenderPass&  getRenderPass () { return m_renderPass; }
		mvPipeline&    getPipeline   () { return m_pipeline; }
		mvCommandPool& getCommandPool() { return m_commandPool; }

	private:

		GLFWwindow*              m_window;
		VkInstance               m_instance;
		VkDebugUtilsMessengerEXT m_debugMessenger;
		mvDevice                 m_device;
		mvSwapChain              m_swapChain;
		mvRenderPass             m_renderPass;
		mvPipeline               m_pipeline;
		mvCommandPool            m_commandPool;
		mvSurface                m_surface;

		// temp
		mvVertexBuffer           m_vertexBuffer;

		std::vector<VkSemaphore> m_imageAvailableSemaphores;
		std::vector<VkSemaphore> m_renderFinishedSemaphores;
		std::vector<VkFence>     m_inFlightFences;
		std::vector<VkFence>     m_imagesInFlight;

	};

	struct Vertex 
	{

		glm::vec2 pos;
		glm::vec3 color;

		static VkVertexInputBindingDescription getBindingDescription()
		{
			VkVertexInputBindingDescription bindingDescription{};
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(Vertex);
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
			return bindingDescription;
		}

		static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions()
		{
			std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[0].offset = offsetof(Vertex, pos);

			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[1].offset = offsetof(Vertex, color);

			return attributeDescriptions;
		}
	};

}