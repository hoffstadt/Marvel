#include "common/transform.hlsli"

cbuffer ShadowTransformCBuf : register(b1)
{
    matrix shadowPosition[3];
};

float4 ToShadowHomoSpace(int i, const in float3 pos, uniform matrix modelTransform)
{
    const float4 world = mul(float4(pos, 1.0f), modelTransform);
    return mul(world, shadowPosition[i]);
}

struct VSOut
{
    float3 viewPos : Position;
    float3 viewNormal : Normal;
    float3 tan : Tangent;
    float3 bitan : Bitangent;
    float2 tc : Texcoord;
    float4 shadowHomoPos[3] : shadowPosition;
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
    vso.shadowHomoPos[0] = ToShadowHomoSpace(0, pos, model);
    vso.shadowHomoPos[1] = ToShadowHomoSpace(1, pos, model);
    vso.shadowHomoPos[2] = ToShadowHomoSpace(2, pos, model);
    return vso;
}