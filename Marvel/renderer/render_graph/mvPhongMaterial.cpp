#include "mvPhongMaterial.h"
#include "mvGraphics.h"
#include "mvCommonBindables.h"
#include "mvCommonBuffers.h"
#include "mvObjMaterial.h"

namespace Marvel {

	mvPhongMaterial::mvPhongMaterial(mvGraphics& graphics, const std::string& path, const mvObjMaterial& material)
	{

		bool hasAlpha = false;
		bool hasColorMap = false;

		mvTechnique phong;
		{
			m_material = std::make_shared<mvPhongMaterialCBuf>(graphics, 1);

			m_material->material.materialColor = glm::vec4(material.diffuseColor, 1.0f);
			m_material->material.specularColor = glm::vec4(material.specularColor, 1.0f);
			m_material->material.materialColor.w = material.dissolve;
			m_material->material.specularGloss = material.specularExponent;

			{

				mvStep step("lambertian");

				// diffuse
				if (!material.diffuseMap.empty())
				{
					m_material->material.useTextureMap = true;
					auto texture = mvBindableRegistry::Request<mvTexture>(graphics, path + material.diffuseMap, 0u);
					step.addBindable(texture);
					m_material->material.hasAlpha = texture->hasAlpha();
					hasAlpha = texture->hasAlpha();
					hasColorMap = true;
				}
				else
				{
					m_material->material.useTextureMap = false;
					m_material->material.hasAlpha = m_material->material.materialColor.a != 1.0f;
				}

				// specular
				if (!material.specularMap.empty())
				{
					auto texture = mvBindableRegistry::Request<mvTexture>(graphics, path + material.specularMap, 1u);
					step.addBindable(texture);
					//hasGlossAlpha = texture->hasAlpha();
					m_material->material.useSpecularMap = true;
				}
				else
					m_material->material.useSpecularMap = false;

				// normals
				if (!material.normalMap.empty())
				{
					auto texture = mvBindableRegistry::Request<mvTexture>(graphics, path + material.normalMap, 2u);
					step.addBindable(texture);
					m_material->material.useNormalMap = true;

				}
				else
					m_material->material.useNormalMap = false;

				//-----------------------------------------------------------------------------
				// additional buffers
				//-----------------------------------------------------------------------------
				step.addBuffer(mvBufferRegistry::GetBuffer("transCBuf"));
				step.addBuffer(m_material);

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
				pipeline.rasterizerStateCull = !m_material->material.hasAlpha;
				pipeline.rasterizerStateHwPCF = false;
				pipeline.rasterizerStateDepthBias = 0;    // not used
				pipeline.rasterizerStateSlopeBias = 0.0f; // not used
				pipeline.rasterizerStateClamp = 1.0f;	  // not used

				// pixel shader stage
				pipeline.pixelShader = graphics.getShaderRoot() + "PhongModel_PS.hlsl";
				pipeline.samplers.push_back({ mvSamplerStateTypeFlags::ANISOTROPIC, mvSamplerStateAddressingFlags::WRAP, 0u, false });
				pipeline.samplers.push_back({ mvSamplerStateTypeFlags::BILINEAR, mvSamplerStateAddressingFlags::CLAMP, 1u, true });
				pipeline.samplers.push_back({ mvSamplerStateTypeFlags::BILINEAR, mvSamplerStateAddressingFlags::CLAMP, 2u, true });

				// output merger stage
				pipeline.depthStencilStateFlags = mvDepthStencilStateFlags::OFF;
				pipeline.blendStateFlags = m_material->material.hasAlpha ? mvBlendStateFlags::ON : mvBlendStateFlags::OFF;

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
			pipeline.viewportWidth = 2000;
			pipeline.viewportHeight = 2000;
			pipeline.rasterizerStateCull = !hasAlpha;
			pipeline.rasterizerStateHwPCF = true;
			pipeline.rasterizerStateDepthBias = 50;
			pipeline.rasterizerStateSlopeBias = 2.0f;
			pipeline.rasterizerStateClamp = 0.1f;

			// pixel shader stage
			if (hasColorMap && hasAlpha)
			{
				if (!material.diffuseMap.empty())
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
				step.addBindable(mvBindableRegistry::Request<mvTexture>(graphics, path + material.diffuseMap, 0u));

			// registers required pipeline
			step.registerPipeline(graphics, pipeline);

			map.addStep(step);
		}

		mvTechnique dmap;
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
			pipeline.viewportWidth = 4000;
			pipeline.viewportHeight = 4000;
			pipeline.rasterizerStateCull = !hasAlpha;
			pipeline.rasterizerStateHwPCF = true;
			pipeline.rasterizerStateDepthBias = 50;
			pipeline.rasterizerStateSlopeBias = 2.0f;
			pipeline.rasterizerStateClamp = 0.1f;

			// pixel shader stage
			if (hasColorMap && hasAlpha)
			{
				if (!material.diffuseMap.empty())
				{
					pipeline.pixelShader = graphics.getShaderRoot() + "PhongShadow_PS.hlsl";
					pipeline.samplers.push_back({ mvSamplerStateTypeFlags::ANISOTROPIC, mvSamplerStateAddressingFlags::WRAP, 0u, false });
				}
			}

			// output merger stage
			pipeline.depthStencilStateFlags = mvDepthStencilStateFlags::OFF;
			pipeline.blendStateFlags = hasAlpha ? mvBlendStateFlags::ON : mvBlendStateFlags::OFF;


			mvStep step("directional_shadow");

			//-----------------------------------------------------------------------------
			// additional buffers
			//-----------------------------------------------------------------------------
			step.addBuffer(mvBufferRegistry::GetBuffer("transCBuf"));

			if (!pipeline.pixelShader.empty())
				step.addBindable(mvBindableRegistry::Request<mvTexture>(graphics, path + material.diffuseMap, 0u));

			// registers required pipeline
			step.registerPipeline(graphics, pipeline);

			dmap.addStep(step);
		}


		m_techniques.push_back(phong);
		m_techniques.push_back(map);
		m_techniques.push_back(dmap);
	}

}