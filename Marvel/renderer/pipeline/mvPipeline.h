#pragma once

#include <vector>
#include <string>
#include <d3d11_1.h>
#include "mvComPtr.h"
#include "mvVertexLayout.h"


namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvGraphics;
	class mvBlendState;
	class mvTopologyState;
	class mvRasterizerState;
	class mvDepthStencilState;
	class mvVertexShader;
	class mvPixelShader;
	class mvGeometryShader;
	class mvInputLayout;
	class mvSamplerState;
	class mvViewport;

	//-----------------------------------------------------------------------------
	// Flags
	//-----------------------------------------------------------------------------
	enum class mvBlendStateFlags
	{
		MV_BLEND_STATE_BLEND_NONE,
		MV_BLEND_STATE_BLEND_ON,
		MV_BLEND_STATE_BLEND_OFF
	};

	enum class mvDepthStencilStateFlags
	{
		MV_DEPTH_STENCIL_STATE_NONE,
		MV_DEPTH_STENCIL_STATE_OFF,
		MV_DEPTH_STENCIL_STATE_WRITE,
		MV_DEPTH_STENCIL_STATE_MASK,
		MV_DEPTH_STENCIL_STATE_DEPTH_OFF,
		MV_DEPTH_STENCIL_STATE_DEPTH_REVERSED,
		MV_DEPTH_STENCIL_STATE_DEPTH_FIRST
	};

	enum class mvSamplerStateTypeFlags
	{
		MV_SAMPLER_STATE_TYPE_NONE,
		MV_SAMPLER_STATE_TYPE_ANISOTROPIC,
		MV_SAMPLER_STATE_TYPE_BILINEAR,
		MV_SAMPLER_STATE_TYPE_POINT
	};

	enum class mvSamplerStateAddressingFlags
	{
		MV_SAMPLER_STATE_ADDRESS_NONE,
		MV_SAMPLER_STATE_ADDRESS_BORDER,
		MV_SAMPLER_STATE_ADDRESS_WRAP,
		MV_SAMPLER_STATE_ADDRESS_MIRROR
	};

	//-----------------------------------------------------------------------------
	// Pipeline Creation Structs
	//-----------------------------------------------------------------------------
	struct mvSamplerStateInfo
	{
		mvSamplerStateTypeFlags       type;
		mvSamplerStateAddressingFlags addressing;
		UINT                          slot;
		bool                          hardwarePCF;
	};

	struct mvPipelineInfo
	{
		std::string                     vertexShader;
		std::string                     geometryShader;
		std::string                     pixelShader;
					        
		D3D11_PRIMITIVE_TOPOLOGY        topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
								        
		mvBlendStateFlags               blendStateFlags = mvBlendStateFlags::MV_BLEND_STATE_BLEND_NONE;
		mvDepthStencilStateFlags        depthStencilStateFlags = mvDepthStencilStateFlags::MV_DEPTH_STENCIL_STATE_NONE;
								       
		bool                            rasterizerStateCull = false;
		bool                            rasterizerStateHwPCF = false;
		int                             rasterizerStateDepthBias = -117;
		float                           rasterizerStateSlopeBias = -117.0f;
		float                           rasterizerStateClamp = -117.0f;
								       
		float                           viewportWidth = -1;
		float                           viewportHeight = -1;

		std::vector<mvSamplerStateInfo> samplers;

		mvVertexLayout                  vertexLayout;
	};



	//-----------------------------------------------------------------------------
	// mvPipeline
	//-----------------------------------------------------------------------------
	class mvPipeline
	{

	public:

		static mvPipeline* Request(mvGraphics& graphics, const mvPipelineInfo& info);
		static std::string GenerateUniqueIdentifier(const mvPipelineInfo& info);

	public:

		void set(mvGraphics& graphics);

		const std::string& getUniqueIdentifier() const { return m_id; }

	private:

		mvPipeline(mvGraphics& graphics, const mvPipelineInfo& info);

	private:

		std::string          m_id;
		mvViewport*          m_viewport = nullptr;
		mvInputLayout*       m_inputLayout = nullptr;
		mvGeometryShader*    m_geometryShader = nullptr;
		mvPixelShader*       m_pixelShader = nullptr;
		mvVertexShader*      m_vertexShader = nullptr;
		mvDepthStencilState* m_depthStencilState = nullptr;
		mvRasterizerState*   m_rasterizerState = nullptr;
		mvTopologyState*     m_topologyState = nullptr;
		mvBlendState*        m_blendState = nullptr;

		std::vector<mvSamplerState*> m_samplerStates;

		

	};

}