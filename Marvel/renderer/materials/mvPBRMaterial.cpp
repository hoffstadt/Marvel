#include "mvPBRMaterial.h"
#include "mvGraphics.h"
#include "mvCommonBindables.h"
#include "mvCommonBuffers.h"
#include "mvObjMaterial.h"

namespace Marvel {

	mvPBRMaterial::mvPBRMaterial(mvGraphics& graphics, const std::string& path, const mvObjMaterial& material)
	{
		bool hasAlpha = false;
		bool hasColorMap = false;

		mvTechnique pbr;
		{
			m_material = std::make_shared<mvPBRMaterialCBuf>(graphics, 1);
			m_material->material.metalness = material.metallic;
			m_material->material.roughness = material.roughness;
			m_material->material.fresnel = material.sheen;


			m_material->material.albedo = glm::vec4(material.diffuseColor, material.dissolve);
			{

				mvStep step("lambertian");

				step.addBindable(std::make_shared<mvCubeTexture>(graphics, "../../Resources/SkyBox", 6));

				// diffuse
				if (!material.diffuseMap.empty())
				{
					m_material->material.useAlbedoMap = true;
					auto texture = mvBindableRegistry::Request<mvTexture>(graphics, path + material.diffuseMap, 0u);
					step.addBindable(texture);
					//pbrMaterial->material.hasAlpha = texture->hasAlpha();
				}
				else
				{
					m_material->material.useAlbedoMap = false;
				}

				// normals
				if (!material.normalMap.empty())
				{
					auto texture = mvBindableRegistry::Request<mvTexture>(graphics, path + material.normalMap, 1u);
					step.addBindable(texture);
					//hasGlossAlpha = texture->hasAlpha();
					m_material->material.useNormalMap = true;
				}
				else
					m_material->material.useNormalMap = false;

				// roughness
				if (!material.roughnessMap.empty())
				{
					auto texture = mvBindableRegistry::Request<mvTexture>(graphics, path + material.roughnessMap, 2u);
					step.addBindable(texture);
					m_material->material.useRoughnessMap = true;

				}
				else
					m_material->material.useRoughnessMap = false;

				// metalness
				if (!material.metallicMap.empty())
				{
					auto texture = mvBindableRegistry::Request<mvTexture>(graphics, path + material.metallicMap, 5u);
					step.addBindable(texture);
					m_material->material.useMetalMap = true;

				}
				else
					m_material->material.useMetalMap = false;

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
				pipeline.rasterizerStateCull = m_material->material.albedo.w == 1.0f;
				pipeline.rasterizerStateHwPCF = false;
				pipeline.rasterizerStateDepthBias = 0;    // not used
				pipeline.rasterizerStateSlopeBias = 0.0f; // not used
				pipeline.rasterizerStateClamp = 0.0f;	  // not used

				// pixel shader stage
				pipeline.pixelShader = graphics.getShaderRoot() + "PBRModel_PS.hlsl";
				pipeline.samplers.push_back({ mvSamplerStateTypeFlags::ANISOTROPIC, mvSamplerStateAddressingFlags::WRAP, 0u, false });
				pipeline.samplers.push_back({ mvSamplerStateTypeFlags::BILINEAR, mvSamplerStateAddressingFlags::CLAMP, 1u, true });
				pipeline.samplers.push_back({ mvSamplerStateTypeFlags::BILINEAR, mvSamplerStateAddressingFlags::CLAMP, 2u, true });
				pipeline.samplers.push_back({ mvSamplerStateTypeFlags::ANISOTROPIC, mvSamplerStateAddressingFlags::WRAP, 3u, false });

				// output merger stage
				pipeline.depthStencilStateFlags = mvDepthStencilStateFlags::OFF;
				pipeline.blendStateFlags = m_material->material.albedo.w == 1.0f ? mvBlendStateFlags::ON : mvBlendStateFlags::OFF;


				// registers required pipeline
				step.registerPipeline(graphics, pipeline);

				pbr.addStep(step);
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


		m_techniques.push_back(pbr);
		m_techniques.push_back(map);
		m_techniques.push_back(dmap);
	}

}