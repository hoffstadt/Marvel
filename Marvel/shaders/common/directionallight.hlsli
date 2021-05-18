cbuffer mvDirectionLightCBuf : register(b2)
{
    int dLightCount;
    float3 dambient;
    float3 viewLightDir[2];
    float3 ddiffuseColor[2];
    float ddiffuseIntensity[2];
};

float3 Fog(const in float dist, uniform float fogstart, uniform float fogrange, uniform float3 fogcolor, const in float3 litcolor)
{
    float s = saturate((dist - fogstart) / fogrange);
    return litcolor + s*(fogcolor - litcolor);
}