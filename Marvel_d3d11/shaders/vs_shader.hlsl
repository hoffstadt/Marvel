#include "transform.hlsli"

struct VS_Output
{
    float4 n : Normal;
    float4 pos : SV_POSITION;
};

VS_Output main(float3 pos : Position, float4 n : Normal)
{
    VS_Output output;
    output.pos = mul(float4(pos, 1.0f), modelViewProj);
    output.n = n;
    return output;
}