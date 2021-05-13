
// textures
Texture2D ColorTexture : register(t0);
Texture2D SpecularTexture : register(t1);
Texture2D NormalTexture : register(t2);

// samplers
SamplerState Sampler : register(s0);

void main(float3 viewFragPos : Position, float3 viewNormal : Normal, float3 viewTan : Tangent, float3 viewBitan : Bitangent,
float2 tc : Texcoord)
{
    float4 color = ColorTexture.Sample(Sampler, tc);
    
    clip(color.a < 0.1f ? -1 : 1);
}