#include "common/transform.hlsli"

struct VSOut
{
    float3 viewPos : Position;
    float3 viewNormal : Normal;
    float3 tan : Tangent;
    float3 bitan : Bitangent;
    float2 tc : Texcoord;
    float4 pos : SV_Position;
};

VSOut main(float3 pos : Position, float3 n : Normal, float3 tan : Tangent, float3 bitan : Bitangent, float2 tc : Texcoord)
{
    VSOut vso;
    vso.viewPos = pos;
    vso.viewNormal = n;
    vso.tan = tan;
    vso.bitan = bitan;
    vso.pos =mul(float4(pos, 1.0f), modelViewProj);;
    vso.tc = tc;
    return vso;
}