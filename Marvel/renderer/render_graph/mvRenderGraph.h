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

		mvRenderGraph(mvGraphics& graphics, const char* skybox);

		void    addJob (mvJob job, size_t target);
		void    execute(mvGraphics& graphics) const;
		void    reset();
		mvPass* getPass(const std::string& name);

	private:

		std::vector <std::shared_ptr<mvPass>> m_passes;

	};

}
