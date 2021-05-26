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
    float3 viewPos        : Position;       // frag pos  (view space)
    float3 viewNormal     : Normal;         // frag norm (view space)
    float2 tc             : Texcoord;       // texture coordinates
    float3x3 tangentBasis : TangentBasis;   // tangent basis
    float4 shadowWorldPos : shadowPosition; // light pos (world space)
    float4 pixelPos       : SV_Position;    // frag pos  (screen space)
};

VSOut main(float3 pos : Position, float3 n : Normal, float3 tan : Tangent, float3 bitan : Bitangent, float2 tc : Texcoord)
{
    VSOut vso;
    vso.viewPos = (float3) mul(float4(pos, 1.0f), modelView);
    vso.viewNormal = mul(n, (float3x3) modelView);
    vso.tangentBasis = float3x3(
        mul(tan,   (float3x3) modelView),
        mul(bitan, (float3x3) modelView),
        mul(n,     (float3x3) modelView));
    vso.pixelPos = mul(float4(pos, 1.0f), modelViewProj);
    vso.tc = tc;
    vso.shadowWorldPos = ToShadowHomoSpace(pos, model);
    return vso;
}