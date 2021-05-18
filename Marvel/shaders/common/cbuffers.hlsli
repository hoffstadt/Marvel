
struct mvPointLightManager
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

struct mvDirectionalLightManager
{
    int LightCount;
    //-------------------------- ( 16 bytes )
    
    float3 viewLightDir[2];
    //-------------------------- ( 32 bytes )
    
    float3 diffuseColor[2];
    //-------------------------- ( 32 bytes )
    
    float diffuseIntensity[2];
    //-------------------------- ( 32 bytes )
    
    //-------------------------- ( 3*32 + 16 = 112 bytes )
};

struct mvMaterial
{
    float3 materialColor;
    //-------------------------- ( 16 bytes )
    
    float3 specularColor;
    float specularWeight;
    //-------------------------- ( 16 bytes )
   
    float specularGloss;
    float normalMapWeight;
    bool useTextureMap;
    bool useNormalMap;
    //-------------------------- ( 16 bytes )
    
    bool useSpecularMap;
    bool useGlossAlpha;
    bool hasAlpha;
    //-------------------------- ( 16 bytes )
    //-------------------------- ( 4 * 16 = 64 bytes )
};

struct mvScene
{
    float FogStart;
    float3 FogColor;
    //-------------------------- ( 16 bytes )
    
    float FogRange;
    float3 ambient;
    //-------------------------- ( 16 bytes )
    
    //-------------------------- ( 2*16 = 32 bytes ) 
};