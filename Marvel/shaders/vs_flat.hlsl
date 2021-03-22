#include "transform.hlsli"

struct VS_Output
{
    float4 viewPos : Position;
    float4 viewNormal : Normal;
    float4 pos : SV_Position;
};


VS_Output main(float3 viewFragPos : Position, float3 viewNormal : Normal)
{

	// final color
    VS_Output output;
    
    output.viewPos = mul(float4(viewFragPos, 1.0f), modelView);
    output.viewNormal = mul(viewNormal, modelView);
    output.pos = mul(float4(viewFragPos, 1.0f), modelViewProj);
    return output;
}