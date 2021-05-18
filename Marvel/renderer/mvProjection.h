#pragma once
#include "mvMath.h"
#include "mvFrustum.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvGraphics;
	class mvRenderGraph;

	//-----------------------------------------------------------------------------
	// mvProjection
	//-----------------------------------------------------------------------------
	class mvProjection
	{
	public:

		mvProjection(mvGraphics& graphics, const std::string& name, float width, float height,
			float nearZ, float farZ, bool ortho);

		glm::mat4 getMatrix() const;

		// called when viewport is resized
		void update(int width, int height);

		void linkTechniques(mvRenderGraph& graph) { m_frustum.linkTechniques(graph); }
		void submit(mvRenderGraph& graph) const { m_frustum.submit(graph); }
		void setPos(float x, float y, float z) { m_frustum.setPos(x, y, z); }
		void setRotation(float x, float y, float z) { m_frustum.setRotation(x, y, z); }

	private:

		float m_width;
		float m_height;
		float m_nearZ;
		float m_farZ;
		mvFrustum m_frustum;

	};

}