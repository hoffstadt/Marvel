#include "mvMarvelImporter.h"
#include <fstream>
#include<sstream>
#include <iterator>
#include <filesystem>
#include <mvBase64.h>

namespace Marvel {

	mvMarvelImporter::mvMarvelImporter(const std::string& file)
	{

		std::filesystem::path path = file;
		m_root = path.parent_path().string() + "\\";

		std::ifstream inputStream(file);
		nlohmann::json j;
		inputStream >> j;


		if (j.contains("scenes"))
		{
			for (auto& item : j["scenes"])
				m_scenes.push_back(mvImpScene(item));
		}

		if (j.contains("nodes"))
		{
			for (auto& item : j["nodes"])
				m_nodes.push_back(mvImpNode(item));
		}

		if (j.contains("meshes"))
		{
			for (auto& item : j["meshes"])
				m_meshes.push_back(mvImpMesh(item));
		}

		if (j.contains("materials"))
		{
			for (auto& item : j["materials"])
				m_materials.push_back(mvImpMaterial(item));
		}

		if (j.contains("textures"))
		{
			for (auto& item : j["textures"])
				m_textures.push_back(mvImpTexture(item));

		}

		if (j.contains("samplers"))
		{
			for (auto& item : j["samplers"])
				m_samplers.push_back(mvImpSampler(item));

		}

		if (j.contains("images"))
		{
			for (auto& item : j["images"])
			{
				if(item.contains("uri"))
					m_images.push_back(item["uri"]);

			}
		}

		if (j.contains("buffers"))
		{
			for (auto& item : j["buffers"])
				m_buffers.push_back(mvImpBuffer(item, m_root));

		}

		if (j.contains("bufferViews"))
		{
			for (auto& item : j["bufferViews"])
				m_bufferViews.push_back(mvImpBufferView(item));

		}


		if (j.contains("accessors"))
		{
			for (auto& item : j["accessors"])
				m_accessors.push_back(mvImpAccessor(item));

		}

	}

	mvImpScene::mvImpScene(nlohmann::json j)
	{
		if (j.contains("nodes"))
		{
			for (auto& item : j["nodes"])
				nodes.push_back(item);
		}

	}

	mvImpAccessor::mvImpAccessor(nlohmann::json j)
	{
		if (j.contains("count"))
			count = j["count"];

		if (j.contains("componentType"))
			component_type = j["componentType"];

		if (j.contains("bufferView"))
			buffer_view_index = j["bufferView"];

		if (j.contains("type"))
		{
			std::string strtype = j["type"];
			if (strtype == "SCALAR")
				type = MV_SCALAR;
			
			else if (strtype == "VEC2")
				type = MV_VEC2;

			else if (strtype == "VEC3")
				type = MV_VEC3;

			else if (strtype == "VEC4")
				type = MV_VEC4;
		}

		if (j.contains("name"))
			name = j["name"];

		if (j.contains("max"))
		{
			for (auto& item : j["max"])
				maxes.push_back(item);
		}

		if (j.contains("min"))
		{
			for (auto& item : j["min"])
				mins.push_back(item);
		}
			
	}

	mvImpBufferView::mvImpBufferView(nlohmann::json j)
	{
		if (j.contains("buffer"))
			buffer_index = j["buffer"];

		if (j.contains("byteOffset"))
			byte_offset = j["byteOffset"];

		if (j.contains("byteLength"))
			byte_length = j["byteLength"];

		if (j.contains("byteStride"))
			byte_stride = j["byteStride"];

		if (j.contains("name"))
			name = j["name"];
	}

	mvImpTexture::mvImpTexture(nlohmann::json j)
	{
		if (j.contains("sampler"))
			sampler_index = j["sampler"];

		if (j.contains("source"))
			image_index = j["source"];

		if (j.contains("name"))
			name = j["name"];
	}

	mvImpBuffer::mvImpBuffer(nlohmann::json j, const std::string& root)
	{
		if (j.contains("uri"))
		{
			uri = j["uri"];
			uri = root + uri;
		}

		if (j.contains("byteLength"))
			byte_length = j["byteLength"];
	}

	std::vector<byte> mvImpBuffer::getBufferData() const
	{
		std::ifstream inputStream(uri, std::ios::binary);

		if (inputStream)
		{
			std::vector<byte> buffer(std::istreambuf_iterator<char>(inputStream), {});
			return buffer;
		}

		return {};
	}

	mvImpSampler::mvImpSampler(nlohmann::json j)
	{
		if (j.contains("magFilter"))
			mag_filter = j["magFilter"];

		if (j.contains("minFilter"))
			min_filter = j["minFilter"];

		if (j.contains("wrapS"))
			wrap_s = j["wrapS"];

		if (j.contains("wrapT"))
			wrap_t = j["wrapT"];
	}

	mvImpNode::mvImpNode(nlohmann::json j)
	{
		if (j.contains("name"))
			name = j["name"];

		if (j.contains("mesh"))
			mesh_index = j["mesh"];

		if (j.contains("children"))
		{
			for (auto& item : j["children"])
				children.push_back(item);
		}
	}

	mvImpMesh::mvImpMesh(nlohmann::json j)
	{
		if (j.contains("name"))
			name = j["name"];
		if (j.contains("primitives"))
		{
			j = j["primitives"][0];

			if (j.contains("indices"))
				indices_index = j["indices"];

			if (j.contains("material"))
				material_index = j["material"];

			if (j.contains("attributes"))
			{
				if (j["attributes"].contains("POSITION"))
					attributes.push_back({ MV_POSITION , j["attributes"]["POSITION"] });

				if (j["attributes"].contains("TANGENT"))
					attributes.push_back({ MV_TANGENT , j["attributes"]["TANGENT"] });

				if (j["attributes"].contains("NORMAL"))
					attributes.push_back({ MV_NORMAL , j["attributes"]["NORMAL"] });

				if (j["attributes"].contains("TEXCOORD_0"))
					attributes.push_back({ MV_TEXTCOORD , j["attributes"]["TEXCOORD_0"] });

			}
		}
	}

	mvImpMaterial::mvImpMaterial(nlohmann::json j)
	{
		if (j.contains("pbrMetallicRoughness"))
		{
			if (j["pbrMetallicRoughness"].contains("baseColorTexture"))
			{
				if (j["pbrMetallicRoughness"]["baseColorTexture"].contains("index"))
					base_color_texture = j["pbrMetallicRoughness"]["baseColorTexture"]["index"];
			}

			if (j["pbrMetallicRoughness"].contains("baseColorFactor"))
			{
				base_color_factor[0] = j["pbrMetallicRoughness"]["baseColorFactor"][0];
				base_color_factor[1] = j["pbrMetallicRoughness"]["baseColorFactor"][1];
				base_color_factor[2] = j["pbrMetallicRoughness"]["baseColorFactor"][2];
				base_color_factor[3] = j["pbrMetallicRoughness"]["baseColorFactor"][3];
			}

			if (j["pbrMetallicRoughness"].contains("metallicRoughnessTexture"))
			{
				if (j["pbrMetallicRoughness"]["metallicRoughnessTexture"].contains("index"))
					metallic_roughness_texture = j["pbrMetallicRoughness"]["metallicRoughnessTexture"]["index"];
			}

			if (j["pbrMetallicRoughness"].contains("metallicFactor"))
				metallic_factor = j["pbrMetallicRoughness"]["metallicFactor"];

			if (j["pbrMetallicRoughness"].contains("roughnessFactor"))
				roughness_factor = j["pbrMetallicRoughness"]["roughnessFactor"];
		}

		if (j.contains("normalTexture"))
		{
			if (j["normalTexture"].contains("index"))
				normal_texture = j["normalTexture"]["index"];

			if (j["normalTexture"].contains("scale"))
				normal_texture_scale = j["normalTexture"]["scale"];
		}

		if (j.contains("occlusionTexture"))
		{
			if (j["occlusionTexture"].contains("index"))
				occlusion_texture = j["occlusionTexture"]["index"];

			if (j["occlusionTexture"].contains("scale"))
				occlusion_texture_strength = j["occlusionTexture"]["strength"];
		}

		if (j.contains("emissiveTexture"))
		{
			if (j["emissiveTexture"].contains("index"))
				emissive_texture = j["emissiveTexture"]["index"];
		}

		if (j.contains("emissiveFactor"))
		{

			emissive_factor[0] = j["emissiveFactor"][0];
			emissive_factor[1] = j["emissiveFactor"][1];
			emissive_factor[2] = j["emissiveFactor"][2];
		}

		if (j.contains("doubleSided"))
			double_sided = j["doubleSided"];

		if (j.contains("name"))
			name = j["name"];
	}

}