
struct mvPointLight
{
    float3 viewLightPos;
    //-------------------------- ( 16 bytes )
    
    float3 diffuseColor;
    float diffuseIntensity;
    //-------------------------- ( 16 bytes )
    
    float attConst;
    float attLin;
    float attQuad;
    //-------------------------- ( 16 bytes )
    
    //-------------------------- ( 4*16 = 64 bytes )
};

struct mvDirectionalLight
{
    float diffuseIntensity;
    float3 viewLightDir;
    //-------------------------- ( 16 bytes )
    
    float3 diffuseColor;
    //-------------------------- ( 16 bytes )
    
    //-------------------------- ( 2*16 = 32 bytes )
};

struct mvPhongMaterial
{
    float4 materialColor;
    //-------------------------- ( 16 bytes )
    
    float3 specularColor;
    float specularGloss;
    //-------------------------- ( 16 bytes )
   
    
    float normalMapWeight;
    bool useTextureMap;
    bool useNormalMap;
    bool useSpecularMap;
    //-------------------------- ( 16 bytes )
    
    
    bool useGlossAlpha;
    bool hasAlpha;
    //-------------------------- ( 16 bytes )
    //-------------------------- ( 4 * 16 = 64 bytes )
};

struct mvPBRMaterial
{
    float4 albedo;
    //-------------------------- ( 16 bytes )
    
    float metalness;
    float roughness;
    float radiance;
    float fresnel;
    //-------------------------- ( 16 bytes )
    
    bool useAlbedoMap;
    bool useNormalMap;
    bool useRoughnessMap;
    bool useMetalMap;
    //-------------------------- ( 16 bytes )
   
    //-------------------------- ( 3 * 16 = 48 bytes )
};

struct mvScene
{
    float FogStart;
    float3 FogColor;
    //-------------------------- ( 16 bytes )
    
    float FogRange;
    float3 ambient;
    //-------------------------- ( 16 bytes )
    
    float3 camPos;
    bool useShadows;
    bool useSkybox;
    //-------------------------- ( 16 bytes )
    
    //-------------------------- ( 3*16 = 48 bytes ) 
};