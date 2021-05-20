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
		NONE,
		ON,
		OFF
	};

	enum class mvDepthStencilStateFlags
	{
		NONE,
		OFF,
		WRITE,
		MASK,
		DEPTH_OFF,
		DEPTH_REVERSED,
		DEPTH_FIRST
	};

	enum class mvSamplerStateTypeFlags
	{
		NONE,
		ANISOTROPIC,
		BILINEAR,
		POINT
	};

	enum class mvSamplerStateAddressingFlags
	{
		NONE,
		BORDER,
		WRAP,
		MIRROR
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
		//-----------------------//
		// input assembler stage //
		//-----------------------//
		D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		mvVertexLayout           vertexLayout;

		//---------------------//
		// vertex shader stage //
		//---------------------//
		std::string vertexShader;

		//-----------------------//
		// geometry shader stage //
		//-----------------------//
		std::string geometryShader;

		//------------------//
		// rasterizer stage	//
		//------------------//
		float viewportWidth            = -1;
		float viewportHeight           = -1;
		bool  rasterizerStateCull      = false;
		bool  rasterizerStateHwPCF     = false;
		int   rasterizerStateDepthBias = -117;
		float rasterizerStateSlopeBias = -117.0f;
		float rasterizerStateClamp     = -117.0f;

		//--------------------//
		// pixel shader stage //
		//--------------------//
		std::string                     pixelShader;
		std::vector<mvSamplerStateInfo> samplers;
					        
		//---------------------//
		// output merger stage //
		//---------------------//
		mvBlendStateFlags        blendStateFlags        = mvBlendStateFlags::NONE;
		mvDepthStencilStateFlags depthStencilStateFlags = mvDepthStencilStateFlags::NONE;

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