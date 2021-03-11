#include "mvCommandPool.h"
#include "mvGraphics.h"

namespace Marvel {

    const std::vector<Vertex> vertices = {
        {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
        };

    void mvCommandPool::init(mvGraphics& graphics)
    {
        createCommandPool(graphics);
    }

    void mvCommandPool::cleanup(mvGraphics& graphics)
    {
        vkDestroyCommandPool(graphics.getDevice().getLogicalDevice(), m_commandPool, nullptr);
    }

    void mvCommandPool::createCommandPool(mvGraphics& graphics)
    {
        QueueFamilyIndices queueFamilyIndices = graphics.getSurface().findQueueFamilies(graphics.getDevice().getPhysicalDevice());

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

        if (vkCreateCommandPool(graphics.getDevice().getLogicalDevice(), &poolInfo, nullptr, &m_commandPool) != VK_SUCCESS)
            throw std::runtime_error("failed to create command pool!");

    }

    void mvCommandPool::createCommandBuffers(mvGraphics& graphics, VkBuffer vertexBuffer)
    {
        m_commandBuffers.resize(graphics.getSwapChain().getFrameBufferCount());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = m_commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = (uint32_t)m_commandBuffers.size();

        if (vkAllocateCommandBuffers(graphics.getDevice().getLogicalDevice(), &allocInfo, m_commandBuffers.data()) != VK_SUCCESS)
            throw std::runtime_error("failed to allocate command buffers!");

        for (size_t i = 0; i < m_commandBuffers.size(); i++)
        {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if (vkBeginCommandBuffer(m_commandBuffers[i], &beginInfo) != VK_SUCCESS)
                throw std::runtime_error("failed to begin recording command buffer!");

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = graphics.getRenderPass().getRenderPass();
            renderPassInfo.framebuffer = graphics.getSwapChain().getFrameBuffer(i);
            renderPassInfo.renderArea.offset = { 0, 0 };
            renderPassInfo.renderArea.extent = graphics.getSwapChain().getExtent();

            VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
            renderPassInfo.clearValueCount = 1;
            renderPassInfo.pClearValues = &clearColor;

            vkCmdBeginRenderPass(m_commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            vkCmdBindPipeline(m_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphics.getPipeline().getPipeline());

            VkBuffer vertexBuffers[] = { vertexBuffer };
            VkDeviceSize offsets[] = { 0 };
            vkCmdBindVertexBuffers(m_commandBuffers[i], 0, 1, vertexBuffers, offsets);

            vkCmdDraw(m_commandBuffers[i], static_cast<uint32_t>(vertices.size()), 1, 0, 0);

            vkCmdEndRenderPass(m_commandBuffers[i]);

            if (vkEndCommandBuffer(m_commandBuffers[i]) != VK_SUCCESS)
                throw std::runtime_error("failed to record command buffer!");
        }
    }

}