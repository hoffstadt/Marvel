#include "mvVertexBuffer.h"
#include "mvGraphics.h"

namespace Marvel {

    const std::vector<Vertex> vertices = {
        {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    };

    void mvVertexBuffer::init(mvGraphics& graphics)
    {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = sizeof(vertices[0]) * vertices.size();
        bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(graphics.getDevice().getLogicalDevice(), &bufferInfo, nullptr, &m_vertexBuffer) != VK_SUCCESS)
            throw std::runtime_error("failed to create vertex buffer!");

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(graphics.getDevice().getLogicalDevice(), m_vertexBuffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = mvDevice::FindMemoryType(graphics.getDevice().getPhysicalDevice(),
            memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        if (vkAllocateMemory(graphics.getDevice().getLogicalDevice(), &allocInfo, nullptr, &m_vertexBufferMemory) != VK_SUCCESS)
            throw std::runtime_error("failed to allocate vertex buffer memory!");


        vkBindBufferMemory(graphics.getDevice().getLogicalDevice(), m_vertexBuffer, m_vertexBufferMemory, 0);

        void* data;
        vkMapMemory(graphics.getDevice().getLogicalDevice(), m_vertexBufferMemory, 0, bufferInfo.size, 0, &data);
        memcpy(data, vertices.data(), (size_t)bufferInfo.size);
        vkUnmapMemory(graphics.getDevice().getLogicalDevice(), m_vertexBufferMemory);
    }

    void mvVertexBuffer::cleanup(mvGraphics& graphics)
    {
        vkDestroyBuffer(graphics.getDevice().getLogicalDevice(), m_vertexBuffer, nullptr);
        vkFreeMemory(graphics.getDevice().getLogicalDevice(), m_vertexBufferMemory, nullptr);
    }



}