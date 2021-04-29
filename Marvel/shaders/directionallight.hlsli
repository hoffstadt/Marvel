cbuffer mvDirectionLightCBuf : register(b2)
{
    int dLightCount;
    float3 dambient;
    float3 viewLightDir[16];
    float3 ddiffuseColor[16];
    float ddiffuseIntensity[16];
};

cbuffer mvFogCBuf : register(b3)
{
    float FogStart;
    float FogRange;
    float3 FogColor;
};

float3 Fog(const in float dist, uniform float fogstart, uniform float fogrange, const in float3 fogcolor, const in float3 litcolor)
{
    float s = saturate((dist - fogstart) / fogrange);
    return litcolor + s*(fogcolor - litcolor);
}