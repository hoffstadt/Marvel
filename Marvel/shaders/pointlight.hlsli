cbuffer mvPointLightCBuf : register(b0)
{
    int LightCount;
    float3 ambient;
    float3 viewLightPos[16];
    float3 diffuseColor[16];
    float diffuseIntensity[16];
    float attConst[16];
    float attLin[16];
    float attQuad[16];
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