cbuffer mvPointLightCBuf : register(b0)
{
    int LightCount;
    //-------------------------- ( 16 bytes )
    
    float3 viewLightPos[3];
    //-------------------------- ( 48 bytes )
    
    float3 diffuseColor[3];
    //-------------------------- ( 48 bytes )
    
    float diffuseIntensity[3];
    //-------------------------- ( 48 bytes )
    
    float attConst[3];
    //-------------------------- ( 48 bytes )
    
    float attLin[3];
    //-------------------------- ( 48 bytes )
    
    float attQuad[3];
    //-------------------------- ( 48 bytes )
    
    //-------------------------- ( 6*48 + 16 = 304 bytes )
};

struct LightVectorData
{
    float3 vec;
    float3 dir;
    float dist;
};

cbuffer mvSceneCBuf : register(b3)
{
    
    float FogStart;
    float3 FogColor;
    //-------------------------- ( 16 bytes )
    
    float FogRange;
    float3 ambient;
    //-------------------------- ( 16 bytes )
    
    //-------------------------- ( 2*16 = 32 bytes )
};

LightVectorData CalculateLightVectorData(const in float3 lightPos, const in float3 fragPos)
{
    LightVectorData lv;
    lv.vec = lightPos - fragPos;
    lv.dist = length(lv.vec);
    lv.dir = lv.vec / lv.dist;
    return lv;
}