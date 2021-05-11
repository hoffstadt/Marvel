#pragma once
#include "mvMath.h"
#include "mvProjection.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvGraphics;
	class mvRenderGraph;

	//-----------------------------------------------------------------------------
	// mvCamera
	//-----------------------------------------------------------------------------
	class mvCamera
	{

	public:

		mvCamera(mvGraphics& graphics, const std::string& name, glm::vec3 homePos = { 0.0f,0.0f, 0.0f },
			float homePitch = 0.0f, float homeYaw = 0.0f, float width = 1.0f, float height = 1.0f, float nearZ = 0.5f, float farZ = 400.0f);

		glm::mat4 getMatrix() const;

		void bind     (mvGraphics& graphics) const;
		void rotate   (float dx, float dy);
		void translate(float dx, float dy, float dz);
		void setPos   (float x, float y, float z);
		void setRotation(float x, float y, float z);
		void linkTechniques(mvRenderGraph& graph);
		void submit(mvRenderGraph& graph) const;
		void show_imgui_windows();

		// called when the viewport is resized
		void updateProjection(int width, int height);

	private:

		glm::vec3    m_pos;
		float        m_pitch = 0.0f;
		float        m_yaw = 0.0f;
		mvProjection m_proj;

		static constexpr float s_travelSpeed = 12.0f;
		static constexpr float s_rotationSpeed = 0.004f;

	};

}