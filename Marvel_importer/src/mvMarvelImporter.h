#pragma once

#include <string>
#include <vector>
#include <json.hpp>

namespace Marvel{

	using byte = std::uint8_t;

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	struct mvImpScene;
	struct mvImpNode;
	struct mvImpMesh;
	struct mvImpAttribute;
	struct mvImpMaterial;
	struct mvImpTexture;
	struct mvImpSampler;
	struct mvImpBuffer;
	struct mvImpBufferView;
	struct mvImpAccessor;
	struct mvImpScene;

	//-----------------------------------------------------------------------------
	// enums
	//-----------------------------------------------------------------------------

	enum mvImpPrimitiveMode {
		MV_POINTS    = 0,
		MV_LINES     = 1,
		MV_TRIANGLES = 4
	};

	enum mvImpAccessorType {
		MV_SCALAR,
		MV_VEC2,
		MV_VEC3,
		MV_VEC4,
	};

	enum mvImpComponentType {
       MV_BYTE           = 0x1400,
       MV_UNSIGNED_BYTE  = 0x1401,
       MV_SHORT          = 0x1402,
       MV_UNSIGNED_SHORT = 0x1403,
       MV_INT            = 0x1404,
       MV_UNSIGNED_INT   = 0x1405,
       MV_FLOAT          = 0x1406,
       MV_DOUBLE         = 0x140A
	};

	enum mvImpAttributeType {
		MV_POSITION = 0,
		MV_TANGENT,
		MV_NORMAL,
		MV_TEXTCOORD,
	};

	//-----------------------------------------------------------------------------
	// mvImpAttribute
	//-----------------------------------------------------------------------------
	struct mvImpAttribute
	{
		mvImpAttributeType type;
		int                index = -1; // accessor index

	};

	//-----------------------------------------------------------------------------
	// mvImpMesh
	//-----------------------------------------------------------------------------
	struct mvImpMesh
	{
		std::string                 name;
		mvImpPrimitiveMode          mode           = MV_TRIANGLES;
		int                         indices_index  = -1; // accessor index
		int                         material_index = -1;
		std::vector<mvImpAttribute> attributes;

		mvImpMesh(nlohmann::json j);
	};

	//-----------------------------------------------------------------------------
	// mvImpNode
	//-----------------------------------------------------------------------------
	struct mvImpNode
	{
		std::string      name;
		int              mesh_index = -1;
		std::vector<int> children;

		mvImpNode(nlohmann::json j);

	};

	//-----------------------------------------------------------------------------
	// mvImpMaterial
	//-----------------------------------------------------------------------------
	struct mvImpMaterial
	{
		std::string name;
		int         base_color_texture         = -1;
		int         metallic_roughness_texture = -1;
		int         normal_texture             = -1;
		int         occlusion_texture          = -1;
		int         emissive_texture           = -1;
		float       normal_texture_scale       = 0.8f;
		float       occlusion_texture_strength = 0.9f;
		float       metallic_factor            = 1.0f;
		float       roughness_factor           = 0.0f;
		float       base_color_factor[4]       = { 1.0f, 1.0f, 1.0f, 1.0f };
		float       emissive_factor[3]         = { 0.0f, 0.0f, 0.0f};
		bool        double_sided               = false;

		mvImpMaterial(nlohmann::json j);
	};

	//-----------------------------------------------------------------------------
	// mvImpTexture
	//-----------------------------------------------------------------------------
	struct mvImpTexture
	{
		std::string name;
		int         image_index = -1;
		int         sampler_index = -1;

		mvImpTexture(nlohmann::json j);
	};

	//-----------------------------------------------------------------------------
	// mvImpSampler
	//-----------------------------------------------------------------------------
	struct mvImpSampler
	{
		int mag_filter = -1;
		int min_filter = -1;
		int wrap_s     = -1;
		int wrap_t     = -1;

		mvImpSampler(nlohmann::json j);
	};

	//-----------------------------------------------------------------------------
	// mvImpBuffer
	//-----------------------------------------------------------------------------
	struct mvImpBuffer
	{
		int         byte_length = -1;
		std::string uri;

		mvImpBuffer(nlohmann::json j, const std::string& root);

		std::vector<byte> getBufferData() const;
	};

	//-----------------------------------------------------------------------------
	// mvImpBufferView
	//-----------------------------------------------------------------------------
	struct mvImpBufferView
	{
		std::string name;
		int         buffer_index = -1;
		int         byte_offset = 0;
		int         byte_length = -1;
		int         byte_stride = -1;

		mvImpBufferView(nlohmann::json j);
	};

	//-----------------------------------------------------------------------------
	// mvImpAccessor
	//-----------------------------------------------------------------------------
	struct mvImpAccessor
	{
		std::string        name;
		mvImpAccessorType  type = MV_SCALAR;
		int                buffer_view_index = -1;
		mvImpComponentType component_type = MV_FLOAT;
		int                count = -1;
		std::vector<float> maxes;
		std::vector<float> mins;

		mvImpAccessor(nlohmann::json j);
	};

	//-----------------------------------------------------------------------------
	// mvImpScene
	//-----------------------------------------------------------------------------
	struct mvImpScene
	{
		std::vector<int> nodes;
		mvImpScene(nlohmann::json j);
	};

	//-----------------------------------------------------------------------------
	// mvMarvelImporter
	//-----------------------------------------------------------------------------
	class mvMarvelImporter
	{

	public:

		mvMarvelImporter(const std::string& file);

		const std::vector<mvImpScene>&      getScenes     () const { return m_scenes; }
		const std::vector<mvImpBuffer>&     getBuffers    () const { return m_buffers; }
		const std::vector<mvImpBufferView>& getBufferViews() const { return m_bufferViews; }
		const std::vector<mvImpAccessor>&   getAccessors  () const { return m_accessors; }
		const std::vector<mvImpNode>&       getNodes      () const { return m_nodes; }
		const std::vector<mvImpMesh>&       getMeshes     () const { return m_meshes; }
		const std::vector<mvImpMaterial>&   getMaterials  () const { return m_materials; }
		const std::vector<mvImpTexture>&    getTextures   () const { return m_textures; }
		const std::vector<mvImpSampler>&    getSamplers   () const { return m_samplers; }
		const std::vector<std::string>&     getImages     () const {return m_images;}

	private:

		std::string m_root;

		std::vector<mvImpScene>      m_scenes;
		std::vector<mvImpBuffer>     m_buffers;
		std::vector<mvImpBufferView> m_bufferViews;
		std::vector<mvImpAccessor>   m_accessors;
		std::vector<mvImpNode>       m_nodes;
		std::vector<mvImpMesh>       m_meshes;
		std::vector<mvImpMaterial>   m_materials;
		std::vector<mvImpTexture>    m_textures;
		std::vector<mvImpSampler>    m_samplers;
		std::vector<std::string>     m_images;

	};

}