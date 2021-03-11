#include "mvSwapChain.h"
#include "mvGraphics.h"

namespace Marvel {

	void mvSwapChain::init(mvGraphics& graphics)
	{
        createSwapChain(graphics);
        createImageViews(graphics);
	}

	void mvSwapChain::createSwapChain(mvGraphics& graphics)
	{
        SwapChainSupportDetails swapChainSupport = graphics.getSurface().querySwapChainSupport(graphics.getDevice().getPhysicalDevice());

        // choose swap surface Format
        VkSurfaceFormatKHR surfaceFormat = swapChainSupport.formats[0];
        for (const auto& availableFormat : swapChainSupport.formats)
        {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                surfaceFormat = availableFormat;
                break;
            }
        }

        // chose swap present mode
        VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
        for (const auto& availablePresentMode : swapChainSupport.presentModes)
        {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                presentMode = availablePresentMode;
                break;
            }
        }

        // chose swap extent
        VkExtent2D extent;
        if (swapChainSupport.capabilities.currentExtent.width != UINT32_MAX)
            extent = swapChainSupport.capabilities.currentExtent;
        else
        {
            int width, height;
            glfwGetFramebufferSize(graphics.getWindow(), &width, &height);

            VkExtent2D actualExtent = {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)
            };

            actualExtent.width = std::max(swapChainSupport.capabilities.minImageExtent.width, std::min(swapChainSupport.capabilities.maxImageExtent.width, actualExtent.width));
            actualExtent.height = std::max(swapChainSupport.capabilities.minImageExtent.height, std::min(swapChainSupport.capabilities.maxImageExtent.height, actualExtent.height));

            extent = actualExtent;
        }

        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
        if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
            imageCount = swapChainSupport.capabilities.maxImageCount;

        {
            VkSwapchainCreateInfoKHR createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
            createInfo.surface = graphics.getSurface().getSurface();

            createInfo.minImageCount = imageCount;
            createInfo.imageFormat = surfaceFormat.format;
            createInfo.imageColorSpace = surfaceFormat.colorSpace;
            createInfo.imageExtent = extent;
            createInfo.imageArrayLayers = 1;
            createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

            QueueFamilyIndices indices = graphics.getSurface().findQueueFamilies(graphics.getDevice().getPhysicalDevice());
            uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

            if (indices.graphicsFamily != indices.presentFamily)
            {
                createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
                createInfo.queueFamilyIndexCount = 2;
                createInfo.pQueueFamilyIndices = queueFamilyIndices;
            }
            else
                createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

            createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
            createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
            createInfo.presentMode = presentMode;
            createInfo.clipped = VK_TRUE;

            createInfo.oldSwapchain = VK_NULL_HANDLE;

            if (vkCreateSwapchainKHR(graphics.getDevice().getLogicalDevice(), &createInfo, nullptr, &m_swapChain) != VK_SUCCESS)
                throw std::runtime_error("failed to create swap chain!");
        }

        vkGetSwapchainImagesKHR(graphics.getDevice().getLogicalDevice(), m_swapChain, &imageCount, nullptr);
        m_swapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(graphics.getDevice().getLogicalDevice(), m_swapChain, &imageCount, m_swapChainImages.data());

        m_swapChainImageFormat = surfaceFormat.format;
        m_swapChainExtent = extent;
	}

    void mvSwapChain::createImageViews(mvGraphics& graphics)
    {
        m_swapChainImageViews.resize(m_swapChainImages.size());

        for (size_t i = 0; i < m_swapChainImages.size(); i++)
        {
            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = m_swapChainImages[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = m_swapChainImageFormat;
            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(graphics.getDevice().getLogicalDevice(), &createInfo, nullptr, &m_swapChainImageViews[i]) != VK_SUCCESS)
                throw std::runtime_error("failed to create image views!");
        }
    }

    void mvSwapChain::createFrameBuffers(mvGraphics& graphics)
    {
        m_swapChainFramebuffers.resize(m_swapChainImageViews.size());

        for (size_t i = 0; i < m_swapChainImageViews.size(); i++)
        {
            VkImageView attachments[] = {
                m_swapChainImageViews[i]
            };

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = graphics.getRenderPass().getRenderPass();
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments = attachments;
            framebufferInfo.width = m_swapChainExtent.width;
            framebufferInfo.height = m_swapChainExtent.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(graphics.getDevice().getLogicalDevice(), &framebufferInfo, nullptr, &m_swapChainFramebuffers[i]) != VK_SUCCESS)
                throw std::runtime_error("failed to create framebuffer!");
        }
    }

    void mvSwapChain::cleanup(mvGraphics& graphics)
    {
        for (auto framebuffer : m_swapChainFramebuffers)
            vkDestroyFramebuffer(graphics.getDevice().getLogicalDevice(), framebuffer, nullptr);

        vkFreeCommandBuffers(graphics.getDevice().getLogicalDevice(), graphics.getCommandPool().getPool(), 
            static_cast<uint32_t>(graphics.getCommandPool().getCommandBuffers().size()), 
            graphics.getCommandPool().getCommandBuffers().data());

        vkDestroyPipeline(graphics.getDevice().getLogicalDevice(), graphics.getPipeline().getPipeline(), nullptr);
        vkDestroyPipelineLayout(graphics.getDevice().getLogicalDevice(), graphics.getPipeline().getLayout(), nullptr);
        vkDestroyRenderPass(graphics.getDevice().getLogicalDevice(), graphics.getRenderPass().getRenderPass(), nullptr);

        for (auto imageView : m_swapChainImageViews)
            vkDestroyImageView(graphics.getDevice().getLogicalDevice(), imageView, nullptr);

        vkDestroySwapchainKHR(graphics.getDevice().getLogicalDevice(), m_swapChain, nullptr);
    }
}