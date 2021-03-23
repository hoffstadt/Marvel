#pragma once

#include <vector>
#include <memory>
#include "mvPass.h"
#include "mvVertexBuffer.h"
#include "mvIndexBuffer.h"

namespace Marvel {

	class mvGraphics;

	class mvRenderGraph
	{

	public:

		mvRenderGraph(mvGraphics& graphics);

		void addJob(mvJob job, size_t target);

		void execute(mvGraphics& graphics) const;

		void reset();

	public:

		std::vector<mvPass>             m_passes;
		std::shared_ptr<mvVertexBuffer> m_vertexBuffer;
		std::shared_ptr<mvIndexBuffer>  m_indexBuffer;

	};

}
