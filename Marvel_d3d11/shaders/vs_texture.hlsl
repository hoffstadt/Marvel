#include "transform.hlsli"

struct VS_Output
{   
    float2 uv : Texcoord;
    float4 pos : SV_POSITION;
};

VS_Output main(float3 pos : Position, float2 uv : Texcoord)
{
    VS_Output output;
    output.pos = mul(float4(pos, 1.0f), modelViewProj);
    output.uv = uv;
    return output;
}