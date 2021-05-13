#include "mvMaterial.h"
#include "mvGraphics.h"
#include "mvCommonBindables.h"
#include "assimp/Importer.hpp"
#include "assimp/Scene.h"
#include "assimp/postprocess.h"

namespace Marvel {

	mvMaterial::mvMaterial(mvGraphics& graphics, const aiMaterial& material, const std::string& path)
	{

		std::shared_ptr<mvPixelConstantBuffer> buf;
		std::unique_ptr<mvBuffer>              bufferRaw;

		mvBufferLayout layout(std::make_shared<mvBufferLayoutEntry>(Struct));
		auto& rootStruct = layout.getRoot();
		rootStruct->add(Float3, std::string("materialColor"));
		rootStruct->add(Float3, std::string("specularColor"));
		rootStruct->add(Float, std::string("specularWeight"));
		rootStruct->add(Float, std::string("specularGloss"));
		rootStruct->add(Float, std::string("normalMapWeight"));
		rootStruct->add(Bool, std::string("useTextureMap"));
		rootStruct->add(Bool, std::string("useNormalMap"));
		rootStruct->add(Bool, std::string("useSpecularMap"));
		rootStruct->add(Bool, std::string("useGlossAlpha"));
		rootStruct->finalize(0);

		bool hasAlpha = false;

		bufferRaw = std::make_unique<mvBuffer>(std::move(layout));

		bufferRaw->getElement("useTextureMap").setIfExists(false);
		bufferRaw->getElement("useNormalMap").setIfExists(false);
		bufferRaw->getElement("useSpecularMap").setIfExists(false);
		bufferRaw->getElement("useGlossAlpha").setIfExists(false);

		bufferRaw->getElement("normalMapWeight").setIfExists(1.0f);
		bufferRaw->getElement("specularWeight").setIfExists(1.0f);
		

		aiColor3D diffuseColor = { 0.45f,0.45f,0.85f };
		material.Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
		bufferRaw->getElement("materialColor") = reinterpret_cast<glm::vec3&>(diffuseColor);

		aiColor3D specularColor = { 0.18f,0.18f,0.18f };
		//aiColor3D specularColor = { 0.0f,0.0f,0.0f };
		material.Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
		bufferRaw->getElement("specularColor") = reinterpret_cast<glm::vec3&>(specularColor);

		float gloss = 8.0f;
		material.Get(AI_MATKEY_SHININESS, gloss);
		bufferRaw->getElement("specularGloss") = gloss;

		{
			mvTechnique phong;
			mvStep step("lambertian");
			aiString texFileName;

			// create vertex layout
			m_layout.append(ElementType::Position3D);
			m_layout.append(ElementType::Normal);
			m_layout.append(ElementType::Tangent);
			m_layout.append(ElementType::Bitangent);
			m_layout.append(ElementType::Texture2D);

			// diffuse
			if (material.GetTexture(aiTextureType_DIFFUSE, 0, &texFileName) == aiReturn_SUCCESS)
			{
				bufferRaw->getElement("useTextureMap").setIfExists(true);
				auto texture = mvBindableRegistry::Request<mvTexture>(graphics, path + texFileName.C_Str(), 0u);
				step.addBindable(texture);
				hasAlpha = texture->hasAlpha();
			}
				

			step.addBindable(mvBindableRegistry::Request<mvRasterizer>(graphics, hasAlpha));

			// specular
			if (material.GetTexture(aiTextureType_SPECULAR, 0, &texFileName) == aiReturn_SUCCESS)
			{
				auto texture = mvBindableRegistry::Request<mvTexture>(graphics, path + texFileName.C_Str(), 1u);
				step.addBindable(texture);
				//hasGlossAlpha = texture->hasAlpha();
				bufferRaw->getElement("useSpecularMap").setIfExists(true);
			}

			// normals
			if (material.GetTexture(aiTextureType_NORMALS, 0, &texFileName) == aiReturn_SUCCESS)
			{
				auto texture = mvBindableRegistry::Request<mvTexture>(graphics, path + texFileName.C_Str(), 2u);
				step.addBindable(texture);
				bufferRaw->getElement("useNormalMap").setIfExists(true);
				
			}

			buf = std::make_shared<mvPixelConstantBuffer>(graphics, *rootStruct.get(), 1, bufferRaw.get());

			// create vertex shader
			auto vshader = mvBindableRegistry::Request<mvVertexShader>(graphics, graphics.getShaderRoot() + "PhongModel_VS.hlsl");

			step.addBindable(vshader);
			step.addBindable(mvBindableRegistry::Request<mvInputLayout>(graphics, m_layout, *vshader));
			step.addBindable(mvBindableRegistry::Request<mvPixelShader>(graphics, graphics.getShaderRoot() + "PhongModel_PS.hlsl"));
			step.addBindable(mvBindableRegistry::GetBindable("transCBuf"));
			step.addBindable(mvBindableRegistry::Request<mvBlender>(graphics, true));
			step.addBindable(mvBindableRegistry::Request<mvSampler>(graphics, mvSampler::Type::Anisotropic, false, 0u));

			step.addBindable(buf);

			phong.addStep(step);
			m_techniques.push_back(phong);
		}

		{
			mvTechnique map;
			mvStep step("shadow");

			// create vertex shader
			auto vshader = mvBindableRegistry::Request<mvVertexShader>(graphics, graphics.getShaderRoot() + "Shadow_VS.hlsl");
			step.addBindable(vshader);
			step.addBindable(mvBindableRegistry::Request<mvInputLayout>(graphics, m_layout, *vshader));
			step.addBindable(std::make_shared<mvTransformConstantBuffer>(graphics));
			map.addStep(step);
			m_techniques.push_back(map);
		}
		
	}

	std::vector<mvTechnique> mvMaterial::getTechniques() const
	{
		return m_techniques;
	}

	const mvVertexLayout& mvMaterial::getLayout() const
	{
		return m_layout;
	}

}