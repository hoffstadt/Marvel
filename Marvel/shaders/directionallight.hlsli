cbuffer mvDirectionLightCBuf : register(b2)
{
    int dLightCount;
    float3 dambient;
    float3 viewLightDir[16];
    float3 ddiffuseColor[16];
    float ddiffuseIntensity[16];
};
