#pragma once
#include <string>
#include <unordered_map>

namespace Marvel {

	class mvNode;
	class mvModel;

	class mvModelProbe
	{

	public:

		mvModelProbe(const std::string& name);
		
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
		};
		std::string m_name;
		std::unordered_map<int, TransformParameters> m_transformParams;

	private:

		TransformParameters& ResolveTransform();
		TransformParameters& LoadTransform(int id);
	};
}