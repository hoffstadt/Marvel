#include "mvObjMaterial.h"
#include <fstream>
#include <sstream>

namespace Marvel {

	mvObjMaterialParser::mvObjMaterialParser(const std::string& file)
	{

		std::ifstream inputStream(file);

		for (std::string line; std::getline(inputStream, line);)
			processLine(line);

	}

	void mvObjMaterialParser::processLine(const std::string& line)
	{

		// ignore comments
		if (line.rfind("#", 0) == 0)
			return;

		std::vector<std::string> tokens;

		std::stringstream stream(line);

		for (std::string token; std::getline(stream, token, ' ');)
			tokens.push_back(token);

		// check if line is empty
		if (tokens.size() < 2)
			return;

		// check if new material
		if (tokens[0] == std::string("newmtl"))
		{
			m_materials.push_back(mvObjMaterial());
			m_currentMaterial = &m_materials.back();
			m_currentMaterial->name = tokens[1];
		}

		else if (tokens[0] == std::string("Ka"))
		{
			m_currentMaterial->ambientColor.r = std::stof(tokens[1]);
			m_currentMaterial->ambientColor.g = std::stof(tokens[2]);
			m_currentMaterial->ambientColor.b = std::stof(tokens[3]);
		}

		else if (tokens[0] == std::string("Kd"))
		{
			m_currentMaterial->diffuseColor.r = std::stof(tokens[1]);
			m_currentMaterial->diffuseColor.g = std::stof(tokens[2]);
			m_currentMaterial->diffuseColor.b = std::stof(tokens[3]);
		}

		else if (tokens[0] == std::string("Ks"))
		{
			m_currentMaterial->specularColor.r = std::stof(tokens[1]);
			m_currentMaterial->specularColor.g = std::stof(tokens[2]);
			m_currentMaterial->specularColor.b = std::stof(tokens[3]);
		}

		else if (tokens[0] == std::string("Tf"))
		{
			m_currentMaterial->transmissionFilter.r = std::stof(tokens[1]);
			m_currentMaterial->transmissionFilter.g = std::stof(tokens[2]);
			m_currentMaterial->transmissionFilter.b = std::stof(tokens[3]);
		}

		else if (tokens[0] == std::string("Ke"))
		{
			m_currentMaterial->emissive.r = std::stof(tokens[1]);
			m_currentMaterial->emissive.g = std::stof(tokens[2]);
			m_currentMaterial->emissive.b = std::stof(tokens[3]);
		}

		else if (tokens[0] == std::string("Ni"))
			m_currentMaterial->opticalDensity = std::stof(tokens[1]);

		else if (tokens[0] == std::string("Pr"))
		{
			m_currentMaterial->roughness = std::stof(tokens[1]);
			m_currentMaterial->pbr = true;
		}

		else if (tokens[0] == std::string("Pm"))
		{
			m_currentMaterial->metallic = std::stof(tokens[1]);
			m_currentMaterial->pbr = true;
		}

		else if (tokens[0] == std::string("Ps"))
		{
			m_currentMaterial->sheen = std::stof(tokens[1]);
			m_currentMaterial->pbr = true;
		}

		else if (tokens[0] == std::string("Ns"))
			m_currentMaterial->specularExponent = std::stof(tokens[1]);

		else if (tokens[0] == std::string("d"))
			m_currentMaterial->dissolve = std::stof(tokens[1]);

		else if (tokens[0] == std::string("Tr"))
			m_currentMaterial->dissolve = 1-std::stof(tokens[1]);

		else if (tokens[0] == std::string("map_Ka"))
			m_currentMaterial->ambientMap = tokens[1];

		else if (tokens[0] == std::string("map_Kd"))
			m_currentMaterial->diffuseMap = tokens[1];

		else if (tokens[0] == std::string("map_Kn"))
			m_currentMaterial->normalMap = tokens[1];

		else if (tokens[0] == std::string("map_Ks"))
			m_currentMaterial->specularMap = tokens[1];

		else if (tokens[0] == std::string("map_Ke"))
			m_currentMaterial->emissiveMap = tokens[1];

		else if (tokens[0] == std::string("map_Ns"))
			m_currentMaterial->specularExponentMap = tokens[1];

		else if (tokens[0] == std::string("map_d"))
			m_currentMaterial->alphaMap = tokens[1];

		else if (tokens[0] == std::string("map_bump"))
			m_currentMaterial->bumpMap = tokens[1];

		else if (tokens[0] == std::string("map_Pr"))
		{
			m_currentMaterial->roughnessMap = tokens[1];
			m_currentMaterial->pbr = true;
		}

		else if (tokens[0] == std::string("map_Pm"))
		{
			m_currentMaterial->metallicMap = tokens[1];
			m_currentMaterial->pbr = true;
		}

		else if (tokens[0] == std::string("map_Ps"))
		{
			m_currentMaterial->sheenMap = tokens[1];
			m_currentMaterial->pbr = true;
		}

		else if (tokens[0] == std::string("disp"))
			m_currentMaterial->displacementMap = tokens[1];
	}

	const std::vector<mvObjMaterial>& mvObjMaterialParser::getMaterials() const
	{
		return m_materials;
	}

	mvObjParser::mvObjParser(const std::string& file)
	{

		std::ifstream inputStream(file);

		for (std::string line; std::getline(inputStream, line);)
			processLine(line);

		postProcess();
	}

	const mvObjNode& mvObjParser::getRootNode() const
	{
		return m_rootNode;
	}

	const std::vector<mvObjMesh*>& mvObjParser::getMeshes() const
	{
		return m_meshes;
	}

	void mvObjParser::postProcess()
	{
		for (auto mesh : m_meshes)
		{
			for (size_t i = 0; i < mesh->triangleCount; i++)
			{
				size_t i0 = mesh->indicies[i * 3];
				size_t i1 = mesh->indicies[i * 3 + 1];
				size_t i2 = mesh->indicies[i * 3 + 2];

				glm::vec3 pos0 = mesh->averticies[i0].position;
				glm::vec3 pos1 = mesh->averticies[i1].position;
				glm::vec3 pos2 = mesh->averticies[i2].position;

				glm::vec2 tex0 = mesh->averticies[i0].uv;
				glm::vec2 tex1 = mesh->averticies[i1].uv;
				glm::vec2 tex2 = mesh->averticies[i2].uv;

				glm::vec3 edge1 = pos1 - pos0;
				glm::vec3 edge2 = pos2 - pos0;

				glm::vec2 uv1 = tex1 - tex0;
				glm::vec2 uv2 = tex2 - tex0;

				float dirCorrection = (uv1.x * uv2.y - uv1.y * uv2.x) < 0.0f ? -1.0f : 1.0f;

				if (uv1.x * uv2.y == uv1.y * uv2.x)
				{
					uv1.x = 0.0f;
					uv1.y = 1.0f;
					uv2.x = 1.0f;
					uv2.y = 0.0f;
				}

				glm::vec3 tangent = {
					((edge1.x * uv2.y) - (edge2.x * uv1.y)) * dirCorrection,
					((edge1.y * uv2.y) - (edge2.y * uv1.y)) * dirCorrection,
					((edge1.z * uv2.y) - (edge2.z * uv1.y)) * dirCorrection
				};

				glm::vec3 bitangent = {
					((edge1.x * uv2.x) - (edge2.x * uv1.x)) * dirCorrection,
					((edge1.t * uv2.x) - (edge2.y * uv1.x)) * dirCorrection,
					((edge1.z * uv2.x) - (edge2.z * uv1.x)) * dirCorrection
				};

				// project tangent and bitangent into the plane formed by the vertex' normal
				mesh->averticies[i0].tangent = tangent - mesh->averticies[i0].normal * (tangent * mesh->averticies[i0].normal);
				mesh->averticies[i1].tangent = tangent - mesh->averticies[i1].normal * (tangent * mesh->averticies[i1].normal);
				mesh->averticies[i2].tangent = tangent - mesh->averticies[i2].normal * (tangent * mesh->averticies[i2].normal);
				mesh->averticies[i0].bitangent = bitangent - mesh->averticies[i0].normal * (bitangent * mesh->averticies[i0].normal);
				mesh->averticies[i1].bitangent = bitangent - mesh->averticies[i1].normal * (bitangent * mesh->averticies[i1].normal);
				mesh->averticies[i2].bitangent = bitangent - mesh->averticies[i2].normal * (bitangent * mesh->averticies[i2].normal);

				// normalize
				mesh->averticies[i0].tangent = glm::normalize(mesh->averticies[i0].tangent);
				mesh->averticies[i1].tangent = glm::normalize(mesh->averticies[i1].tangent);
				mesh->averticies[i2].tangent = glm::normalize(mesh->averticies[i2].tangent);
				mesh->averticies[i0].bitangent = glm::normalize(mesh->averticies[i0].bitangent);
				mesh->averticies[i1].bitangent = glm::normalize(mesh->averticies[i1].bitangent);
				mesh->averticies[i2].bitangent = glm::normalize(mesh->averticies[i2].bitangent);
			}

			// left hand
			for (size_t i = 0; i < mesh->triangleCount; i++)
			{
				size_t i0 = mesh->indicies[i * 3];
				size_t i1 = mesh->indicies[i * 3 + 1];
				size_t i2 = mesh->indicies[i * 3 + 2];

				mesh->indicies[i * 3] = i2;
				mesh->indicies[i * 3 + 2] = i0;


				mesh->averticies[i0].position.z *= -1.0f;
				mesh->averticies[i1].position.z *= -1.0f;
				mesh->averticies[i2].position.z *= -1.0f;

				mesh->averticies[i0].normal.z *= -1.0f;
				mesh->averticies[i1].normal.z *= -1.0f;
				mesh->averticies[i2].normal.z *= -1.0f;

				mesh->averticies[i0].tangent.z *= -1.0f;
				mesh->averticies[i1].tangent.z *= -1.0f;
				mesh->averticies[i2].tangent.z *= -1.0f;

				mesh->averticies[i0].bitangent.z *= -1.0f;
				mesh->averticies[i1].bitangent.z *= -1.0f;
				mesh->averticies[i2].bitangent.z *= -1.0f;

				mesh->averticies[i0].uv.g = 1 - mesh->averticies[i0].uv.g;
				mesh->averticies[i1].uv.g = 1 - mesh->averticies[i1].uv.g;
				mesh->averticies[i2].uv.g = 1 - mesh->averticies[i2].uv.g;

			}

		}
	}

	void mvObjParser::processLine(const std::string& line)
	{

		// ignore comments
		if (line.rfind("#", 0) == 0)
			return;

		std::vector<std::string> tokens;

		std::stringstream stream(line);

		for (std::string token; std::getline(stream, token, ' ');)
			tokens.push_back(token);

		// check if line is empty
		if (tokens.size() < 2)
			return;

		// check if new mesh
		if (tokens[0] == std::string("o"))
		{
			m_rootNode.name = tokens[1];
			//m_meshes.push_back(new mvObjMesh());
			//m_meshes.back()->name = tokens[1];
		}

		else if (tokens[0] == std::string("g"))
		{
			m_meshes.push_back(new mvObjMesh());
			m_meshes.back()->name = tokens[1];

			m_rootNode.children.push_back({});
			m_rootNode.children.back().name = tokens[1];
			m_rootNode.children.back().meshes.push_back(m_meshes.size() - 1);
		}

		else if (tokens[0] == std::string("mtllib"))
			m_materialLib = tokens[1];

		else if (tokens[0] == std::string("usemtl"))
		{
			m_currentMesh = m_meshes.back();
			m_currentMesh->material = tokens[1];
		}

		else if (tokens[0] == std::string("v"))
		{

			m_verticies.push_back(
				glm::vec3{
					std::stof(tokens[1]),
					std::stof(tokens[2]),
					std::stof(tokens[3])
				});
		}

		else if (tokens[0] == std::string("vn"))
		{

			m_normals.push_back(
				glm::vec3{
					std::stof(tokens[1]),
					std::stof(tokens[2]),
					std::stof(tokens[3])
				});

		}

		else if (tokens[0] == std::string("vt"))
		{

			m_textureCoordinates.push_back(
				glm::vec2{
					std::stof(tokens[1]),
					std::stof(tokens[2])
				});

		}

		else if (tokens[0] == std::string("f"))
		{

			assert(tokens.size() == 4);

			for (int i = 1; i < 4; i++)
			{
				std::vector<std::string> subtokens;

				std::stringstream substream(tokens[i]);

				for (std::string subtoken; std::getline(substream, subtoken, '/');)
					subtokens.push_back(subtoken);

				unsigned int vertexIndex = std::stoul(subtokens[0])-1;
				unsigned int uvIndex = std::stoul(subtokens[1])-1;
				unsigned int normalIndex = std::stoul(subtokens[2])-1;

				m_currentMesh->averticies.push_back({});
				mvObjVertex& vertex = m_currentMesh->averticies.back();
				vertex.position = m_verticies[vertexIndex];
				vertex.normal = m_normals[normalIndex];
				vertex.uv = m_textureCoordinates[uvIndex];

				m_currentMesh->indicies.push_back(m_currentMesh->indicies.size());
				m_currentMesh->vertexCount++;
			}
			m_currentMesh->triangleCount++;
		}

	}

}