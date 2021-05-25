#include "common/transform.hlsli"

cbuffer ShadowTransformCBuf : register(b1)
{
    matrix shadowPosition;
};

float4 ToShadowHomoSpace(const in float3 pos, uniform matrix modelTransform)
{
    const float4 world = mul(float4(pos, 1.0f), modelTransform);
    return mul(world, shadowPosition);
}

struct VSOut
{
    float3 viewPos : Position;
    float3 viewNormal : Normal;
    float3 tan : Tangent;
    float3 bitan : Bitangent;
    float2 tc : Texcoord;
    float4 shadowHomoPos : shadowPosition;
    float4 pos : SV_Position;
};

VSOut main(float3 pos : Position, float3 n : Normal, float3 tan : Tangent, float3 bitan : Bitangent, float2 tc : Texcoord)
{
    VSOut vso;
    vso.viewPos = (float3) mul(float4(pos, 1.0f), modelView);
    vso.viewNormal = mul(n, (float3x3) modelView);
    vso.tan = mul(tan, (float3x3) modelView);
    vso.bitan = mul(bitan, (float3x3) modelView);    
    vso.pos = mul(float4(pos, 1.0f), modelViewProj);
    vso.tc = tc;
    vso.shadowHomoPos = ToShadowHomoSpace(pos, model);
    return vso;
}