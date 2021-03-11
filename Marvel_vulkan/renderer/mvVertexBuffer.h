#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

namespace Marvel {

	class mvGraphics;

	class mvVertexBuffer
	{

	public:

		void init(mvGraphics& graphics);
		void cleanup(mvGraphics& graphics);

		VkBuffer getVertexBuffer() { return m_vertexBuffer; }


	private:

		VkBuffer                     m_vertexBuffer;
		VkDeviceMemory               m_vertexBufferMemory;

	};

}