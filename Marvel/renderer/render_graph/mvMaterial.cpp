#include "mvMaterial.h"
#include "mvGraphics.h"
#include "mvCommonBindables.h"
#include "assimp/Importer.hpp"
#include "assimp/Scene.h"
#include "assimp/postprocess.h"

namespace Marvel {

	mvMaterial::mvMaterial(mvGraphics& graphics, const aiMaterial& material, const std::string& path)
	{
		
		aiColor3D diffuseColor = { 0.45f,0.45f,0.85f };
		material.Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
		m_materialBuffer.materialColor = { diffuseColor.r, diffuseColor.g, diffuseColor.b };

		aiColor3D specularColor = { 0.18f,0.18f,0.18f };
		material.Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
		m_materialBuffer.specularColor = { specularColor.r, specularColor.g, specularColor.b };

		float gloss = 8.0f;
		material.Get(AI_MATKEY_SHININESS, gloss);
		m_materialBuffer.specularGloss = gloss;

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
				m_materialBuffer.useTextureMap = true;
				auto texture = mvBindableRegistry::Request<mvTexture>(graphics, path + texFileName.C_Str(), 0u);
				step.addBindable(texture);
				m_materialBuffer.hasAlpha = texture->hasAlpha();
			}
			else
			{
				m_materialBuffer.useTextureMap = false;
				m_materialBuffer.hasAlpha = false;
			}
				

			step.addBindable(mvBindableRegistry::Request<mvRasterizer>(graphics, m_materialBuffer.hasAlpha));

			// specular
			if (material.GetTexture(aiTextureType_SPECULAR, 0, &texFileName) == aiReturn_SUCCESS)
			{
				auto texture = mvBindableRegistry::Request<mvTexture>(graphics, path + texFileName.C_Str(), 1u);
				step.addBindable(texture);
				//hasGlossAlpha = texture->hasAlpha();
				m_materialBuffer.useSpecularMap = true;
			}
			else
			{
				m_materialBuffer.useSpecularMap = false;
			}

			// normals
			if (material.GetTexture(aiTextureType_NORMALS, 0, &texFileName) == aiReturn_SUCCESS)
			{
				auto texture = mvBindableRegistry::Request<mvTexture>(graphics, path + texFileName.C_Str(), 2u);
				step.addBindable(texture);
				m_materialBuffer.useNormalMap = true;
				
			}
			else
			{
				m_materialBuffer.useNormalMap = false;
			}

			// create vertex shader
			auto vshader = mvBindableRegistry::Request<mvVertexShader>(graphics, graphics.getShaderRoot() + "PhongModel_VS.hlsl");

			step.addBindable(vshader);
			step.addBindable(mvBindableRegistry::Request<mvInputLayout>(graphics, m_layout, *vshader));
			step.addBindable(mvBindableRegistry::Request<mvPixelShader>(graphics, graphics.getShaderRoot() + "PhongModel_PS.hlsl"));
			step.addBindable(mvBindableRegistry::GetBindable("transCBuf"));
			step.addBindable(mvBindableRegistry::Request<mvRasterizer>(graphics, m_materialBuffer.hasAlpha));
			step.addBindable(mvBindableRegistry::Request<mvBlender>(graphics, m_materialBuffer.hasAlpha));
			step.addBindable(mvBindableRegistry::Request<mvSampler>(graphics, mvSampler::Type::Anisotropic, false, 0u));

			step.addBindable(std::make_shared<mvPixelConstantBuffer>(graphics, 1u, &m_materialBuffer));

			phong.addStep(step);
			m_techniques.push_back(phong);
		}

		{
			mvTechnique map;
			mvStep step("shadow");

			// create vertex shader
			auto vshader = mvBindableRegistry::Request<mvVertexShader>(graphics, graphics.getShaderRoot() + "PhongShadow_VS.hlsl");
			step.addBindable(vshader);
			step.addBindable(mvBindableRegistry::Request<mvInputLayout>(graphics, m_layout, *vshader));
			step.addBindable(mvBindableRegistry::Request<mvBlender>(graphics, m_materialBuffer.hasAlpha));
			if (m_materialBuffer.useTextureMap && m_materialBuffer.hasAlpha)
			{
				aiString texFileName;
				if (material.GetTexture(aiTextureType_DIFFUSE, 0, &texFileName) == aiReturn_SUCCESS)
				{
					auto texture = mvBindableRegistry::Request<mvTexture>(graphics, path + texFileName.C_Str(), 0u);
					step.addBindable(texture);
					step.addBindable(mvBindableRegistry::Request<mvPixelShader>(graphics, graphics.getShaderRoot() + "PhongShadow_PS.hlsl"));
					step.addBindable(mvBindableRegistry::Request<mvSampler>(graphics, mvSampler::Type::Anisotropic, false, 0u));
				}

			}
			else
			{
				step.addBindable(mvBindableRegistry::GetBindable("null_ps"));
			}
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