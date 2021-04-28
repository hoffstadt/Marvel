#include "mvCommonBindables.h"
#include <assert.h>
#include "mvGraphics.h"

namespace Marvel {

	std::unordered_map<std::string, std::shared_ptr<mvBindable>> mvBindableRegistry::m_bindables;

	void mvBindableRegistry::Initialize(mvGraphics& graphics)
	{
		// pixel shaders
		m_bindables.try_emplace("Phong_PS", std::make_shared<mvPixelShader>(graphics, std::string(graphics.getShaderRoot() + "Phong_PS.hlsl").c_str()));
		//m_bindables.try_emplace("PhongDif_PS", std::make_shared<mvPixelShader>(graphics, std::string(graphics.getShaderRoot() + "PhongDif_PS.hlsl").c_str()));
		//m_bindables.try_emplace("PhongDifMskSpc_PS", std::make_shared<mvPixelShader>(graphics, std::string(graphics.getShaderRoot() + "PhongDifMskSpc_PS.hlsl").c_str()));
		//m_bindables.try_emplace("PhongDifMskSpcNrm_PS", std::make_shared<mvPixelShader>(graphics, std::string(graphics.getShaderRoot() + "PhongDifMskSpcNrm_PS.hlsl").c_str()));
		//m_bindables.try_emplace("PhongDifMskNrm_PS", std::make_shared<mvPixelShader>(graphics, std::string(graphics.getShaderRoot() + "PhongDifNrm_PS.hlsl").c_str()));
		//m_bindables.try_emplace("PhongDifNrm_PS", std::make_shared<mvPixelShader>(graphics, std::string(graphics.getShaderRoot() + "PhongDifNrm_PS.hlsl").c_str()));
		//m_bindables.try_emplace("PhongDifSpc_PS", std::make_shared<mvPixelShader>(graphics, std::string(graphics.getShaderRoot() + "PhongDifSpc_PS.hlsl").c_str()));
		//m_bindables.try_emplace("PhongDifSpcNrm_PS", std::make_shared<mvPixelShader>(graphics, std::string(graphics.getShaderRoot() + "PhongDifSpcNrm_PS.hlsl").c_str()));
		m_bindables.try_emplace("Solid_PS", std::make_shared<mvPixelShader>(graphics, std::string(graphics.getShaderRoot() + "Solid_PS.hlsl").c_str()));
		m_bindables.try_emplace("Skybox_PS", std::make_shared<mvPixelShader>(graphics, std::string(graphics.getShaderRoot() + "Skybox_PS.hlsl").c_str()));

		// vertex shaders
		m_bindables.try_emplace("Phong_VS", std::make_shared<mvVertexShader>(graphics, std::string(graphics.getShaderRoot() + "Phong_VS.hlsl").c_str()));
		//m_bindables.try_emplace("PhongDif_VS", std::make_shared<mvVertexShader>(graphics, std::string(graphics.getShaderRoot() + "PhongDif_VS.hlsl").c_str()));
		//m_bindables.try_emplace("PhongDifMskSpc_VS", std::make_shared<mvVertexShader>(graphics, std::string(graphics.getShaderRoot() + "PhongDifMskSpc_VS.hlsl").c_str()));
		//m_bindables.try_emplace("PhongDifMskSpcNrm_VS", std::make_shared<mvVertexShader>(graphics, std::string(graphics.getShaderRoot() + "PhongDifMskSpcNrm_VS.hlsl").c_str()));
		//m_bindables.try_emplace("PhongDifMskNrm_VS", std::make_shared<mvVertexShader>(graphics, std::string(graphics.getShaderRoot() + "PhongDifNrm_VS.hlsl").c_str()));
		//m_bindables.try_emplace("PhongDifNrm_VS", std::make_shared<mvVertexShader>(graphics, std::string(graphics.getShaderRoot() + "PhongDifNrm_VS.hlsl").c_str()));
		//m_bindables.try_emplace("PhongDifSpc_VS", std::make_shared<mvVertexShader>(graphics, std::string(graphics.getShaderRoot() + "PhongDifSpc_VS.hlsl").c_str()));
		//m_bindables.try_emplace("PhongDifSpcNrm_VS", std::make_shared<mvVertexShader>(graphics, std::string(graphics.getShaderRoot() + "PhongDifSpcNrm_VS.hlsl").c_str()));
		m_bindables.try_emplace("Solid_VS", std::make_shared<mvVertexShader>(graphics, std::string(graphics.getShaderRoot() + "Solid_VS.hlsl").c_str()));
		m_bindables.try_emplace("Skybox_VS", std::make_shared<mvVertexShader>(graphics, std::string(graphics.getShaderRoot() + "Skybox_VS.hlsl").c_str()));

		m_bindables.try_emplace("gs_null", std::make_shared<mvNullGeometryShader>(graphics));
		m_bindables.try_emplace("sampler", std::make_shared<mvSampler>(graphics));
		m_bindables.try_emplace("transCBuf", std::make_shared<mvTransformConstantBuffer>(graphics));
		m_bindables.try_emplace("blender", std::make_shared<mvBlender>(graphics, true));
	}

	std::shared_ptr<mvBindable> mvBindableRegistry::GetBindable(const std::string& ID)
	{
		if (m_bindables.find(ID) == m_bindables.end())
		{
			assert(false);
			return nullptr;
		}

		return m_bindables.at(ID);
	}

	std::shared_ptr<mvBindable> mvBindableRegistry::AddBindable(const std::string& ID, std::shared_ptr<mvBindable> bindable)
	{
		assert(m_bindables.find(ID) == m_bindables.end());

		m_bindables.try_emplace(ID, bindable);

		return bindable;
	}

}