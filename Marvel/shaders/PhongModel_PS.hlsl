#include "common/cbuffers.hlsli"
#include "common/equations.hlsli"

//-----------------------------------------------------------------------------
// textures
//-----------------------------------------------------------------------------
Texture2D   ColorTexture         : register(t0);
Texture2D   SpecularTexture      : register(t1);
Texture2D   NormalTexture        : register(t2);
TextureCube ShadowMap            : register(t3);
Texture2D   DirectionalShadowMap : register(t4);

//-----------------------------------------------------------------------------
// samplers
//-----------------------------------------------------------------------------
SamplerState           Sampler        : register(s0);
SamplerComparisonState ShadowSampler  : register(s1);
SamplerState           DShadowSampler : register(s2);

//-----------------------------------------------------------------------------
// constant buffers
//-----------------------------------------------------------------------------
cbuffer mvPointLightCBuf       : register(b0) { mvPointLight PointLight; };
cbuffer mvMaterialCBuf         : register(b1) { mvPhongMaterial material; };
cbuffer mvDirectionalLightCBuf : register(b2) { mvDirectionalLight DirectionalLight; };
cbuffer mvSceneCBuf            : register(b3) { mvScene Scene; };

struct VSOut
{
    float3 viewPos         : Position;        // pixel pos           (view space)
    float3 viewNormal      : Normal;          // pixel norm          (view space)
    float3 worldNormal     : WorldNormal;     // pixel normal        (view space)
    float2 tc              : Texcoord;        // texture coordinates (model space)
    float3x3 tangentBasis  : TangentBasis;    // tangent basis       (view space)
    float4 shadowWorldPos1 : shadowPosition1; // light pos           (world space)
    float4 shadowWorldPos2 : shadowPosition2; // light pos           (world space)
    float4 pixelPos        : SV_Position;     // pixel pos           (screen space)
};

float4 main(VSOut input) : SV_Target
{
    float3 diffuse = { 0.0f, 0.0f, 0.0f };
    float3 specularReflected = { 0.0f, 0.0f, 0.0f };
    float3 specularReflectedColor = material.specularColor;
    float3 materialColor = material.materialColor;
    
    if(material.useTextureMap)
    {
        const float4 dtex = ColorTexture.Sample(Sampler, input.tc);
        materialColor = dtex.rgb;
    }

    if (material.hasAlpha)
    {

        const float4 dtex = ColorTexture.Sample(Sampler, input.tc);
            
        // bail if highly translucent
        clip(dtex.a < 0.1f ? -1 : 1);
        
        // flip normal when backface
        if (dot(input.viewNormal, input.viewPos) >= 0.0f)
        {
            input.viewNormal = -input.viewNormal;
        }
    }

    // normalize the mesh normal
    input.viewNormal = normalize(input.viewNormal);
    
    // replace normal with mapped if normal mapping enabled
    if (material.useNormalMap)
    {
        // sample and unpack the normal from texture into target space   
        const float3 normalSample = NormalTexture.Sample(Sampler, input.tc).xyz;
        const float3 tanNormal = normalSample * 2.0f - 1.0f;
        const float3 mappedNormal = normalize(mul(input.tangentBasis, tanNormal));
        input.viewNormal = lerp(input.viewNormal, mappedNormal, material.normalMapWeight);
    }
    
    // specular parameter determination (mapped or uniform)
    float specularPowerLoaded = material.specularGloss;
    
    if (material.useSpecularMap)
    {
        const float4 specularSample = SpecularTexture.Sample(Sampler, input.tc);
        specularReflectedColor = specularSample.rgb;
        
        if (material.useGlossAlpha)
        {
            specularPowerLoaded = pow(2.0f, specularSample.a * 13.0f);
        }
    }
     
    //-----------------------------------------------------------------------------
    // point light with simple shadows
    //-----------------------------------------------------------------------------
    
    float shadowLevel = Shadow(input.shadowWorldPos1, ShadowMap, ShadowSampler);
    if(!Scene.useShadows)            
        shadowLevel = 1.0f;
    if (shadowLevel != 0.0f)
    {
            
	    // fragment to light vector data
        float3 lightVec = PointLight.viewLightPos - input.viewPos;
        float lightDistFromFrag = length(lightVec);
        float3 lightDirVec = lightVec / lightDistFromFrag;
    
	    // attenuation
        const float att = Attenuate(PointLight.attConst, PointLight.attLin, PointLight.attQuad, lightDistFromFrag);
    
	    // diffuse
        diffuse += PointLight.diffuseColor * PointLight.diffuseIntensity * att * max(0.0f, dot(lightDirVec, input.viewNormal));
    
        // specular
        
        // calculate reflected light vector
        const float3 w = input.viewNormal * dot(lightVec, input.viewNormal);
        const float3 r = normalize(w * 2.0f - lightVec);
        
        // vector from camera to fragment
        const float3 viewCamToFrag = normalize(input.viewPos);
        
        specularReflected += att * PointLight.diffuseColor * PointLight.diffuseIntensity * specularReflectedColor * 1.0f * pow(max(0.0f, dot(-r, viewCamToFrag)), specularPowerLoaded);
            
        // scale by shadow level
        diffuse *= shadowLevel;
        specularReflected *= shadowLevel;
    }
  
    //-----------------------------------------------------------------------------
    // directional light
    //-----------------------------------------------------------------------------
    {
        
        float lightDepthValue;
        float2 projectTexCoord;
        float bias = 0.001f;
        float3 lightDir;
        
        lightDir = -DirectionalLight.viewLightDir;
        
        // Calculate the projected texture coordinates.
        projectTexCoord.x = 0.5f * input.shadowWorldPos2.x / input.shadowWorldPos2.w + 0.5f;
        projectTexCoord.y = -0.5f * input.shadowWorldPos2.y / input.shadowWorldPos2.w + 0.5f;
        
        // Determine if the projected coordinates are in the 0 to 1 range.  If so then this pixel is in the view of the light.
        if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
        {
            shadowLevel = DirectionalShadowMap.Sample(DShadowSampler, projectTexCoord).r;
            
            // Calculate the depth of the light.
            lightDepthValue = input.shadowWorldPos2.z / input.shadowWorldPos2.w;

           // Subtract the bias from the lightDepthValue.
            lightDepthValue = lightDepthValue - bias;
            
            // Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel.
            // If the light is in front of the object then light the pixel, if not then shadow this pixel since an object (occluder) is casting a shadow on it.
            
            // not in shadow
            if (lightDepthValue < shadowLevel)
            {
                
                // diffuse
                //diffuse += shadowLevel * DirectionalLight.diffuseColor * DirectionalLight.diffuseIntensity * max(0.0f, dot(normalize(lightDir), input.viewNormal));
                diffuse += DirectionalLight.diffuseColor * DirectionalLight.diffuseIntensity * max(0.0f, dot(normalize(lightDir), input.viewNormal));
    
                // specular
        
                // calculate reflected light vector
                const float3 w = input.viewNormal * dot(normalize(lightDir), input.viewNormal);
                const float3 r = normalize(w * 2.0f - normalize(lightDir));
        
                // vector from camera to fragment
                const float3 viewCamToFrag = normalize(input.viewPos);
        
                specularReflected += DirectionalLight.diffuseColor * DirectionalLight.diffuseIntensity * specularReflectedColor * 1.0f * pow(max(0.0f, dot(r, viewCamToFrag)), specularPowerLoaded);
            
            }
           
        }
        else
        {
            return float4(1.0f, 0.0f, 0.0f, 1.0f);
             // diffuse
            diffuse += DirectionalLight.diffuseColor * DirectionalLight.diffuseIntensity * max(0.0f, dot(normalize(lightDir), input.viewNormal));
    
            // specular
        
            // calculate reflected light vector
            const float3 w = input.viewNormal * dot(normalize(lightDir), input.viewNormal);
            const float3 r = normalize(w * 2.0f - normalize(lightDir));
        
            // vector from camera to fragment
            const float3 viewCamToFrag = normalize(input.viewPos);
        
            specularReflected += DirectionalLight.diffuseColor * DirectionalLight.diffuseIntensity * specularReflectedColor * 1.0f * pow(max(0.0f, dot(-r, viewCamToFrag)), specularPowerLoaded);
            
        }
        
    }
    //-----------------------------------------------------------------------------
    // final color
    //-----------------------------------------------------------------------------

    float3 litColor = saturate((diffuse + Scene.ambient) * materialColor + specularReflected);   
    return float4(litColor, 1.0f);
    //return float4(Fog(distance(float3(0.0f, 0.0f, 0.0f), viewFragPos), Scene.FogStart, Scene.FogRange, Scene.FogColor, litColor), 1.0f);

}