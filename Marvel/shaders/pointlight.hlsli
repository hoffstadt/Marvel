cbuffer mvPointLightCBuf : register(b0)
{
    float3 viewLightPos;
    float3 ambient;
    float3 diffuseColor;
    float diffuseIntensity;
    float attConst;
    float attLin;
    float attQuad;
};

struct LightVectorData
{
    float3 vec;
    float3 dir;
    float dist;
};

LightVectorData CalculateLightVectorData(const in float3 lightPos, const in float3 fragPos)
{
    LightVectorData lv;
    lv.vec = lightPos - fragPos;
    lv.dist = length(lv.vec);
    lv.dir = lv.vec / lv.dist;
    return lv;
}