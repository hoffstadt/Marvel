#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

namespace Marvel {

	class mvGraphics;

	class mvSwapChain
	{

	public:

		void init(mvGraphics& graphics);
		void createFrameBuffers(mvGraphics& graphics);
		void cleanup(mvGraphics& graphics);

		size_t getImageCount() const { return m_swapChainImages.size(); }
		size_t getFrameBufferCount() const { return m_swapChainFramebuffers.size(); }
		VkFramebuffer getFrameBuffer(int i) { return m_swapChainFramebuffers[i]; }

		VkSwapchainKHR getSwapChain() { return m_swapChain; }
		VkFormat       getFormat   () { return m_swapChainImageFormat; }
		VkExtent2D     getExtent   () { return m_swapChainExtent; }

	private:

		void createSwapChain(mvGraphics& graphics);
		void createImageViews(mvGraphics& graphics);

	private:

		VkSwapchainKHR             m_swapChain;
		std::vector<VkImage>       m_swapChainImages;
		std::vector<VkImageView>   m_swapChainImageViews;
		VkFormat                   m_swapChainImageFormat;
		VkExtent2D                 m_swapChainExtent;
		std::vector<VkFramebuffer> m_swapChainFramebuffers;

	};

}