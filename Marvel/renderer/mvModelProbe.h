#pragma once
#include <string>
#include <unordered_map>


namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvNode;
	class mvModel;
	class mvRenderGraph;
	class mvGraphics;

	class mvModelProbe
	{

	public:

		mvModelProbe(mvGraphics& graphics, const std::string& name);
		
		void spawnWindow(mvModel& model);
		bool pushNode(mvNode& node);
		void popNode(mvNode& node);

	private:

		mvNode* m_selectedNode = nullptr;

		struct TransformParameters
		{
			float xRot = 0.0f;
			float yRot = 0.0f;
			float zRot = 0.0f;
			float x = 0.0f;
			float y = 0.0f;
			float z = 0.0f;

			float oxRot = 0.0f;
			float oyRot = 0.0f;
			float ozRot = 0.0f;
			float ox = 0.0f;
			float oy = 0.0f;
			float oz = 0.0f;
		};
		std::string m_name;
		std::unordered_map<int, TransformParameters> m_transformParams;

	private:

		TransformParameters& ResolveTransform();
		TransformParameters& LoadTransform(int id);
	};
}