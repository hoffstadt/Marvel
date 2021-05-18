cbuffer mvDirectionLightCBuf : register(b2)
{
    int dLightCount;
    //-------------------------- ( 16 bytes )
    
    float3 viewLightDir[2];
    //-------------------------- ( 32 bytes )
    
    float3 ddiffuseColor[2];
    //-------------------------- ( 32 bytes )
    
    float ddiffuseIntensity[2];
    //-------------------------- ( 32 bytes )
    
    //-------------------------- ( 3*32 + 16 = 112 bytes )
};

float3 Fog(const in float dist, uniform float fogstart, uniform float fogrange, uniform float3 fogcolor, const in float3 litcolor)
{
    float s = saturate((dist - fogstart) / fogrange);
    return litcolor + s*(fogcolor - litcolor);
}