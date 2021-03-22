#include "transform.hlsli"

struct VS_Output
{
    float3 viewPos : Position;
    float3 viewNormal : Normal;
    float2 uv : Texcoord;
    float4 pos : SV_POSITION;
};

VS_Output main(float3 pos : Position, float3 n : Normal, float2 uv : Texcoord)
{
    VS_Output output;
    output.viewPos = (float3) mul(float4(pos, 1.0f), modelView);
    output.viewNormal = mul(n, (float3x3) modelView);
    output.pos = mul(float4(pos, 1.0f), modelViewProj);
    output.uv = uv;
    return output;
}
