#include "mvMaterial.h"
#include "mvGraphics.h"
#include "mvCommonBindables.h"
#include "mvCommonBuffers.h"
#include "assimp/Importer.hpp"
#include "assimp/Scene.h"
#include "assimp/postprocess.h"

namespace Marvel {

	mvMaterial::mvMaterial(mvGraphics& graphics, const aiMaterial& material, const std::string& path, bool PBR)
	{
		
		m_pbr = PBR;

		bool hasAlpha = false;
		bool hasColorMap = false;

		mvTechnique phong;

		if (m_pbr)
		{
			m_pbrMaterial = std::make_shared<mvPBRMaterialCBuf>(graphics, 1);

			aiColor3D diffuseColor = { 0.45f,0.45f,0.85f };
			material.Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
			m_pbrMaterial->material.albedo = { diffuseColor.r, diffuseColor.g, diffuseColor.b };

			{

				mvStep step("lambertian");
				aiString texFileName;

				// diffuse
				if (material.GetTexture(aiTextureType_DIFFUSE, 0, &texFileName) == aiReturn_SUCCESS)
				{
					m_pbrMaterial->material.useAlbedoMap = true;
					auto texture = mvBindableRegistry::Request<mvTexture>(graphics, path + texFileName.C_Str(), 0u);
					step.addBindable(texture);
					//pbrMaterial->material.hasAlpha = texture->hasAlpha();
				}
				else
				{
					m_pbrMaterial->material.useAlbedoMap = false;
					//pbrMaterial->material.hasAlpha = false;
				}

				// normals
				if (material.GetTexture(aiTextureType_HEIGHT, 0, &texFileName) == aiReturn_SUCCESS)
				{
					auto texture = mvBindableRegistry::Request<mvTexture>(graphics, path + texFileName.C_Str(), 1u);
					step.addBindable(texture);
					//hasGlossAlpha = texture->hasAlpha();
					m_pbrMaterial->material.useNormalMap = true;
				}
				else
					m_pbrMaterial->material.useNormalMap = false;

				// roughness
				if (material.GetTexture(aiTextureType_NORMALS, 0, &texFileName) == aiReturn_SUCCESS)
				{
					auto texture = mvBindableRegistry::Request<mvTexture>(graphics, path + texFileName.C_Str(), 2u);
					step.addBindable(texture);
					m_pbrMaterial->material.useRoughnessMap = true;

				}
				else
					m_pbrMaterial->material.useRoughnessMap = false;

				// metalness
				if (material.GetTexture(aiTextureType_SPECULAR, 0, &texFileName) == aiReturn_SUCCESS)
				{
					auto texture = mvBindableRegistry::Request<mvTexture>(graphics, path + texFileName.C_Str(), 4u);
					step.addBindable(texture);
					m_pbrMaterial->material.useMetalMap = true;

				}
				else
					m_pbrMaterial->material.useMetalMap = false;

				//-----------------------------------------------------------------------------
				// additional buffers
				//-----------------------------------------------------------------------------
				step.addBuffer(mvBufferRegistry::GetBuffer("transCBuf"));
				step.addBuffer(m_pbrMaterial);

				//-----------------------------------------------------------------------------
				// pipeline state setup
				//-----------------------------------------------------------------------------
				mvPipelineInfo pipeline;

				// input assembler stage
				pipeline.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
				pipeline.vertexLayout.append(ElementType::Position3D);
				pipeline.vertexLayout.append(ElementType::Normal);
				pipeline.vertexLayout.append(ElementType::Tangent);
				pipeline.vertexLayout.append(ElementType::Bitangent);
				pipeline.vertexLayout.append(ElementType::Texture2D);

				// vertex shader stage
				pipeline.vertexShader = graphics.getShaderRoot() + "PhongModel_VS.hlsl";

				// geometry shader stage
				pipeline.geometryShader = "";

				// rasterizer stage
				pipeline.viewportWidth = 0;  // use render target
				pipeline.viewportHeight = 0; // use render target
				pipeline.rasterizerStateCull = true;
				pipeline.rasterizerStateHwPCF = false;
				pipeline.rasterizerStateDepthBias = 0;    // not used
				pipeline.rasterizerStateSlopeBias = 0.0f; // not used
				pipeline.rasterizerStateClamp = 0.0f;	  // not used

				// pixel shader stage
				pipeline.pixelShader = graphics.getShaderRoot() + "PBRModel_PS.hlsl";
				pipeline.samplers.push_back({ mvSamplerStateTypeFlags::ANISOTROPIC, mvSamplerStateAddressingFlags::WRAP, 0u, false });
				pipeline.samplers.push_back({ mvSamplerStateTypeFlags::POINT, mvSamplerStateAddressingFlags::BORDER, 1u, true });

				// output merger stage
				pipeline.depthStencilStateFlags = mvDepthStencilStateFlags::OFF;
				pipeline.blendStateFlags = mvBlendStateFlags::OFF;

				// registers required pipeline
				step.registerPipeline(graphics, pipeline);

				phong.addStep(step);
				m_layout = pipeline.vertexLayout;
			}
		}

		else
		{
			m_phongMaterial = std::make_shared<mvPhongMaterialCBuf>(graphics, 1);

			aiColor3D diffuseColor = { 0.45f,0.45f,0.85f };
			material.Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
			m_phongMaterial->material.materialColor = { diffuseColor.r, diffuseColor.g, diffuseColor.b };

			aiColor3D specularColor = { 0.18f,0.18f,0.18f };
			material.Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
			m_phongMaterial->material.specularColor = { specularColor.r, specularColor.g, specularColor.b };

			float gloss = 8.0f;
			material.Get(AI_MATKEY_SHININESS, gloss);
			m_phongMaterial->material.specularGloss = gloss;

			
			{

				mvStep step("lambertian");
				aiString texFileName;

				// diffuse
				if (material.GetTexture(aiTextureType_DIFFUSE, 0, &texFileName) == aiReturn_SUCCESS)
				{
					m_phongMaterial->material.useTextureMap = true;
					auto texture = mvBindableRegistry::Request<mvTexture>(graphics, path + texFileName.C_Str(), 0u);
					step.addBindable(texture);
					m_phongMaterial->material.hasAlpha = texture->hasAlpha();
					hasAlpha = texture->hasAlpha();
					hasColorMap = true;
				}
				else
				{
					m_phongMaterial->material.useTextureMap = false;
					m_phongMaterial->material.hasAlpha = false;
				}

				// specular
				if (material.GetTexture(aiTextureType_SPECULAR, 0, &texFileName) == aiReturn_SUCCESS)
				{
					auto texture = mvBindableRegistry::Request<mvTexture>(graphics, path + texFileName.C_Str(), 1u);
					step.addBindable(texture);
					//hasGlossAlpha = texture->hasAlpha();
					m_phongMaterial->material.useSpecularMap = true;
				}
				else
					m_phongMaterial->material.useSpecularMap = false;

				// normals
				if (material.GetTexture(aiTextureType_NORMALS, 0, &texFileName) == aiReturn_SUCCESS)
				{
					auto texture = mvBindableRegistry::Request<mvTexture>(graphics, path + texFileName.C_Str(), 2u);
					step.addBindable(texture);
					m_phongMaterial->material.useNormalMap = true;

				}
				else
					m_phongMaterial->material.useNormalMap = false;

				//-----------------------------------------------------------------------------
				// additional buffers
				//-----------------------------------------------------------------------------
				step.addBuffer(mvBufferRegistry::GetBuffer("transCBuf"));
				step.addBuffer(m_phongMaterial);

				//-----------------------------------------------------------------------------
				// pipeline state setup
				//-----------------------------------------------------------------------------
				mvPipelineInfo pipeline;

				// input assembler stage
				pipeline.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
				pipeline.vertexLayout.append(ElementType::Position3D);
				pipeline.vertexLayout.append(ElementType::Normal);
				pipeline.vertexLayout.append(ElementType::Tangent);
				pipeline.vertexLayout.append(ElementType::Bitangent);
				pipeline.vertexLayout.append(ElementType::Texture2D);

				// vertex shader stage
				pipeline.vertexShader = graphics.getShaderRoot() + "PhongModel_VS.hlsl";

				// geometry shader stage
				pipeline.geometryShader = "";

				// rasterizer stage
				pipeline.viewportWidth = 0;  // use render target
				pipeline.viewportHeight = 0; // use render target
				pipeline.rasterizerStateCull = !m_phongMaterial->material.hasAlpha;
				pipeline.rasterizerStateHwPCF = false;
				pipeline.rasterizerStateDepthBias = 0;    // not used
				pipeline.rasterizerStateSlopeBias = 0.0f; // not used
				pipeline.rasterizerStateClamp = 0.0f;	  // not used

				// pixel shader stage
				pipeline.pixelShader = graphics.getShaderRoot() + "PhongModel_PS.hlsl";
				pipeline.samplers.push_back({ mvSamplerStateTypeFlags::ANISOTROPIC, mvSamplerStateAddressingFlags::WRAP, 0u, false });
				pipeline.samplers.push_back({ mvSamplerStateTypeFlags::POINT, mvSamplerStateAddressingFlags::BORDER, 1u, true });

				// output merger stage
				pipeline.depthStencilStateFlags = mvDepthStencilStateFlags::OFF;
				pipeline.blendStateFlags = m_phongMaterial->material.hasAlpha ? mvBlendStateFlags::ON : mvBlendStateFlags::OFF;

				// registers required pipeline
				step.registerPipeline(graphics, pipeline);

				phong.addStep(step);
				m_layout = pipeline.vertexLayout;
			}

		}
		

		mvTechnique map;
		{
			//-----------------------------------------------------------------------------
			// shadow mapping pipeline state setup
			//-----------------------------------------------------------------------------
			mvPipelineInfo pipeline;

			// input assembler stage
			pipeline.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			pipeline.vertexLayout = m_layout;

			// vertex shader stage
			pipeline.vertexShader = graphics.getShaderRoot() + "PhongShadow_VS.hlsl";

			// geometry shader stage
			pipeline.geometryShader = "";

			// rasterizer stage
			pipeline.viewportWidth = 1000;
			pipeline.viewportHeight = 1000;
			pipeline.rasterizerStateCull = !hasAlpha;
			pipeline.rasterizerStateHwPCF = true;
			pipeline.rasterizerStateDepthBias = 50;
			pipeline.rasterizerStateSlopeBias = 2.0f;
			pipeline.rasterizerStateClamp = 0.1f;

			// pixel shader stage
			aiString texFileName;
			if (hasColorMap && hasAlpha)
			{
				if (material.GetTexture(aiTextureType_DIFFUSE, 0, &texFileName) == aiReturn_SUCCESS)
				{
					pipeline.pixelShader = graphics.getShaderRoot() + "PhongShadow_PS.hlsl";
					pipeline.samplers.push_back({ mvSamplerStateTypeFlags::ANISOTROPIC, mvSamplerStateAddressingFlags::WRAP, 0u, false });
				}
			}

			// output merger stage
			pipeline.depthStencilStateFlags = mvDepthStencilStateFlags::OFF;
			pipeline.blendStateFlags = hasAlpha ? mvBlendStateFlags::ON : mvBlendStateFlags::OFF;


			mvStep step("shadow");

			//-----------------------------------------------------------------------------
			// additional buffers
			//-----------------------------------------------------------------------------
			step.addBuffer(mvBufferRegistry::GetBuffer("transCBuf"));

			if (!pipeline.pixelShader.empty())
				step.addBindable(mvBindableRegistry::Request<mvTexture>(graphics, path + texFileName.C_Str(), 0u));

			// registers required pipeline
			step.registerPipeline(graphics, pipeline);

			map.addStep(step);
		}

		
		m_techniques.push_back(phong);
		m_techniques.push_back(map);		
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