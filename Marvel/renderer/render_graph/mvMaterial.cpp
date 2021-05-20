#include "mvMaterial.h"
#include "mvGraphics.h"
#include "mvCommonBindables.h"
#include "mvCommonBuffers.h"
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

			mvPipelineInfo pipeline;
			pipeline.vertexShader = graphics.getShaderRoot() + "PhongModel_VS.hlsl";
			pipeline.pixelShader = graphics.getShaderRoot() + "PhongModel_PS.hlsl";
			pipeline.geometryShader = "";
			pipeline.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			pipeline.depthStencilStateFlags = mvDepthStencilStateFlags::MV_DEPTH_STENCIL_STATE_OFF;
			pipeline.vertexLayout = m_layout;
			pipeline.rasterizerStateCull = !m_materialBuffer.hasAlpha;
		    pipeline.blendStateFlags = m_materialBuffer.hasAlpha ? mvBlendStateFlags::MV_BLEND_STATE_BLEND_ON : mvBlendStateFlags::MV_BLEND_STATE_BLEND_OFF;

			mvSamplerStateInfo sampler
			{
				mvSamplerStateTypeFlags::MV_SAMPLER_STATE_TYPE_ANISOTROPIC,
				mvSamplerStateAddressingFlags::MV_SAMPLER_STATE_ADDRESS_WRAP,
				0u,
				false
			};

			mvSamplerStateInfo shadowSampler
			{
				mvSamplerStateTypeFlags::MV_SAMPLER_STATE_TYPE_POINT,
				mvSamplerStateAddressingFlags::MV_SAMPLER_STATE_ADDRESS_BORDER,
				1u,
				true
			};

			pipeline.samplers.push_back(sampler);
			pipeline.samplers.push_back(shadowSampler);
			step.registerPipeline(graphics, pipeline);

			step.addBuffer(mvBufferRegistry::GetBuffer("transCBuf"));
			step.addBuffer(std::make_shared<mvPixelConstantBuffer>(graphics, 1u, &m_materialBuffer));

			phong.addStep(step);
			m_techniques.push_back(phong);
		}

		{
			mvTechnique map;
			{
				mvStep step("shadow1");

				mvPipelineInfo pipeline;
				pipeline.viewportWidth = 1000;
				pipeline.viewportHeight = 1000;
				pipeline.vertexShader = graphics.getShaderRoot() + "PhongShadow_VS.hlsl";
				pipeline.geometryShader = "";
				pipeline.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
				pipeline.depthStencilStateFlags = mvDepthStencilStateFlags::MV_DEPTH_STENCIL_STATE_OFF;
				pipeline.vertexLayout = m_layout;
				pipeline.blendStateFlags = m_materialBuffer.hasAlpha ? mvBlendStateFlags::MV_BLEND_STATE_BLEND_ON : mvBlendStateFlags::MV_BLEND_STATE_BLEND_OFF;
				pipeline.rasterizerStateCull = !m_materialBuffer.hasAlpha;
				pipeline.rasterizerStateHwPCF = true;
				pipeline.rasterizerStateDepthBias = 50;
				pipeline.rasterizerStateSlopeBias = 2.0f;
				pipeline.rasterizerStateClamp = 0.1f;

				if (m_materialBuffer.useTextureMap && m_materialBuffer.hasAlpha)
				{
					aiString texFileName;
					if (material.GetTexture(aiTextureType_DIFFUSE, 0, &texFileName) == aiReturn_SUCCESS)
					{
						auto texture = mvBindableRegistry::Request<mvTexture>(graphics, path + texFileName.C_Str(), 0u);
						step.addBindable(texture);
						pipeline.pixelShader = graphics.getShaderRoot() + "PhongShadow_PS.hlsl";

						mvSamplerStateInfo sampler
						{
							mvSamplerStateTypeFlags::MV_SAMPLER_STATE_TYPE_ANISOTROPIC,
							mvSamplerStateAddressingFlags::MV_SAMPLER_STATE_ADDRESS_WRAP,
							0u,
							false
						};

						pipeline.samplers.push_back(sampler);
					}

				}

				step.registerPipeline(graphics, pipeline);

				step.addBuffer(mvBufferRegistry::GetBuffer("transCBuf"));

				map.addStep(step);
			}

			{
				mvStep step("shadow2");

				mvPipelineInfo pipeline;
				pipeline.viewportWidth = 1000;
				pipeline.viewportHeight = 1000;
				pipeline.vertexShader = graphics.getShaderRoot() + "PhongShadow_VS.hlsl";
				pipeline.geometryShader = "";
				pipeline.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
				pipeline.depthStencilStateFlags = mvDepthStencilStateFlags::MV_DEPTH_STENCIL_STATE_OFF;
				pipeline.vertexLayout = m_layout;
				pipeline.blendStateFlags = m_materialBuffer.hasAlpha ? mvBlendStateFlags::MV_BLEND_STATE_BLEND_ON : mvBlendStateFlags::MV_BLEND_STATE_BLEND_OFF;
				pipeline.rasterizerStateCull = !m_materialBuffer.hasAlpha;
				pipeline.rasterizerStateHwPCF = true;
				pipeline.rasterizerStateDepthBias = 50;
				pipeline.rasterizerStateSlopeBias = 2.0f;
				pipeline.rasterizerStateClamp = 0.1f;

				if (m_materialBuffer.useTextureMap && m_materialBuffer.hasAlpha)
				{
					aiString texFileName;
					if (material.GetTexture(aiTextureType_DIFFUSE, 0, &texFileName) == aiReturn_SUCCESS)
					{
						auto texture = mvBindableRegistry::Request<mvTexture>(graphics, path + texFileName.C_Str(), 0u);
						step.addBindable(texture);
						pipeline.pixelShader = graphics.getShaderRoot() + "PhongShadow_PS.hlsl";

						mvSamplerStateInfo sampler
						{
							mvSamplerStateTypeFlags::MV_SAMPLER_STATE_TYPE_ANISOTROPIC,
							mvSamplerStateAddressingFlags::MV_SAMPLER_STATE_ADDRESS_WRAP,
							0u,
							false
						};

						pipeline.samplers.push_back(sampler);
					}

				}

				step.registerPipeline(graphics, pipeline);

				step.addBuffer(mvBufferRegistry::GetBuffer("transCBuf"));

				map.addStep(step);
			}

			{
				mvStep step("shadow3");

				mvPipelineInfo pipeline;
				pipeline.viewportWidth = 1000;
				pipeline.viewportHeight = 1000;
				pipeline.vertexShader = graphics.getShaderRoot() + "PhongShadow_VS.hlsl";
				pipeline.geometryShader = "";
				pipeline.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
				pipeline.depthStencilStateFlags = mvDepthStencilStateFlags::MV_DEPTH_STENCIL_STATE_OFF;
				pipeline.vertexLayout = m_layout;
				pipeline.blendStateFlags = m_materialBuffer.hasAlpha ? mvBlendStateFlags::MV_BLEND_STATE_BLEND_ON : mvBlendStateFlags::MV_BLEND_STATE_BLEND_OFF;
				pipeline.rasterizerStateCull = !m_materialBuffer.hasAlpha;
				pipeline.rasterizerStateHwPCF = true;
				pipeline.rasterizerStateDepthBias = 50;
				pipeline.rasterizerStateSlopeBias = 2.0f;
				pipeline.rasterizerStateClamp = 0.1f;

				if (m_materialBuffer.useTextureMap && m_materialBuffer.hasAlpha)
				{
					aiString texFileName;
					if (material.GetTexture(aiTextureType_DIFFUSE, 0, &texFileName) == aiReturn_SUCCESS)
					{
						auto texture = mvBindableRegistry::Request<mvTexture>(graphics, path + texFileName.C_Str(), 0u);
						step.addBindable(texture);
						pipeline.pixelShader = graphics.getShaderRoot() + "PhongShadow_PS.hlsl";

						mvSamplerStateInfo sampler
						{
							mvSamplerStateTypeFlags::MV_SAMPLER_STATE_TYPE_ANISOTROPIC,
							mvSamplerStateAddressingFlags::MV_SAMPLER_STATE_ADDRESS_WRAP,
							0u,
							false
						};

						pipeline.samplers.push_back(sampler);
					}

				}

				step.registerPipeline(graphics, pipeline);

				step.addBuffer(mvBufferRegistry::GetBuffer("transCBuf"));

				map.addStep(step);
			}

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