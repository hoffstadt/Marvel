#pragma once

#include <string>
#include <vector>
#include "mvMath.h"

namespace Marvel {

	struct mvObjMaterial
	{
		std::string name;

		glm::vec3 ambientColor       = { 1.0f, 1.0f, 1.0f}; // Ka
		glm::vec3 diffuseColor       = { 1.0f, 1.0f, 1.0f}; // Kd
		glm::vec3 specularColor      = { 1.0f, 1.0f, 1.0f}; // Ks
		glm::vec3 transmissionFilter = { 1.0f, 1.0f, 1.0f}; // Tf

		float opticalDensity   = 1.0f; // Ni
		float specularExponent = 0.0f; // Ns
		float dissolve         = 1.0f; // d

		std::string ambientMap = "";          // map_Ka
		std::string diffuseMap = "";          // map_Kd
		std::string normalMap = "";           // map_Kn
		std::string specularMap = "";         // map_Ks
		std::string specularExponentMap = ""; // map_Ns
		std::string alphaMap = "";            // map_d
		std::string bumpMap = "";             // map_bump
		std::string displacementMap = "";     // disp

	};

	struct mvObjVertex
	{
		glm::vec3 position = {};
		glm::vec3 normal = {};
		glm::vec3 tangent = {};
		glm::vec3 bitangent = {};
		glm::vec2 uv = {};
	};

	struct mvObjMesh
	{
		std::string name;
		std::string material;
		unsigned int vertexCount = 0u;
		unsigned int triangleCount = 0u;

		std::vector<mvObjVertex> averticies;
		std::vector<unsigned int> indicies;

	};

	struct mvObjNode
	{
		std::string name;
		std::vector<mvObjNode> children;
		std::vector<unsigned int> meshes;
	};

	class mvObjMaterialParser
	{

	public:

		mvObjMaterialParser(const std::string& file);
		const std::vector<mvObjMaterial>& getMaterials() const;

	private:

		void processLine(const std::string& line);

	private:

		mvObjMaterial* m_currentMaterial = nullptr;
		std::vector<mvObjMaterial> m_materials;

	};

	class mvObjParser
	{

	public:

		mvObjParser(const std::string& file);
		const mvObjNode& getRootNode() const;
		const std::vector<mvObjMesh*>& getMeshes() const;

	private:

		void processLine(const std::string& line);
		void postProcess();

	private:

		mvObjMesh* m_currentMesh = nullptr;
		std::vector<mvObjMesh*> m_meshes;
		mvObjNode m_rootNode;

		std::vector<glm::vec3> m_verticies;
		std::vector<glm::vec3> m_normals;
		std::vector<glm::vec2> m_textureCoordinates;

	};

}