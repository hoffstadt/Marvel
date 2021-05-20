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

		mvTechnique phong;
		{
			
			mvStep step("lambertian");
			aiString texFileName;

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
				m_materialBuffer.useSpecularMap = false;

			// normals
			if (material.GetTexture(aiTextureType_NORMALS, 0, &texFileName) == aiReturn_SUCCESS)
			{
				auto texture = mvBindableRegistry::Request<mvTexture>(graphics, path + texFileName.C_Str(), 2u);
				step.addBindable(texture);
				m_materialBuffer.useNormalMap = true;
				
			}
			else
				m_materialBuffer.useNormalMap = false;

			//-----------------------------------------------------------------------------
			// additional buffers
			//-----------------------------------------------------------------------------
			step.addBuffer(mvBufferRegistry::GetBuffer("transCBuf"));
			step.addBuffer(std::make_shared<mvPixelConstantBuffer>(graphics, 1u, &m_materialBuffer));

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
			pipeline.rasterizerStateCull = !m_materialBuffer.hasAlpha;
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
		    pipeline.blendStateFlags = m_materialBuffer.hasAlpha ? mvBlendStateFlags::ON : mvBlendStateFlags::OFF;

			// registers required pipeline
			step.registerPipeline(graphics, pipeline);

			phong.addStep(step);
			m_layout = pipeline.vertexLayout;
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
			pipeline.rasterizerStateCull = !m_materialBuffer.hasAlpha;
			pipeline.rasterizerStateHwPCF = true;
			pipeline.rasterizerStateDepthBias = 50;
			pipeline.rasterizerStateSlopeBias = 2.0f;
			pipeline.rasterizerStateClamp = 0.1f;

			// pixel shader stage
			aiString texFileName;
			if (m_materialBuffer.useTextureMap && m_materialBuffer.hasAlpha)
			{
				if (material.GetTexture(aiTextureType_DIFFUSE, 0, &texFileName) == aiReturn_SUCCESS)
				{
					pipeline.pixelShader = graphics.getShaderRoot() + "PhongShadow_PS.hlsl";
					pipeline.samplers.push_back({ mvSamplerStateTypeFlags::ANISOTROPIC, mvSamplerStateAddressingFlags::WRAP, 0u, false });
				}
			}

			// output merger stage
			pipeline.depthStencilStateFlags = mvDepthStencilStateFlags::OFF;
			pipeline.blendStateFlags = m_materialBuffer.hasAlpha ? mvBlendStateFlags::ON : mvBlendStateFlags::OFF;

			for (int i = 1; i < 4; i++)
			{
				mvStep step("shadow" + std::to_string(i));

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