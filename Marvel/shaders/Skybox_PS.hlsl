#include "common/cbuffers.hlsli"
TextureCube tex : register(t0);
SamplerState sam : register(s0);
cbuffer mvSceneCBuf: register(b3) { mvScene Scene; };

float4 main(float3 worldPos : Position) : SV_TARGET
{
    if(Scene.useSkybox)
        return tex.Sample(sam, worldPos);
    return float4(0.0f, 0.0f, 0.0f, 1.0f);
}