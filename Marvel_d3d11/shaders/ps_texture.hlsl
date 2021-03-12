
Texture2D mytexture : register(t0);
SamplerState mysampler : register(s0);

float4 main(float2 uv : Texcoord) : SV_Target
{
    return float4(mytexture.Sample(mysampler, uv).rgb, 1.0f);
}