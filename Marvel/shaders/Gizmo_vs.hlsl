#include "common/transform.hlsli"

struct VSOut
{
    float4 Color : Color;
    float4 pos : SV_Position;
};

VSOut main(float3 pos : Position, float4 color : Color)
{
    VSOut vso;
    vso.Color = color;
    vso.pos = mul(float4(pos, 1.0f), modelViewProj);
	return vso;
}