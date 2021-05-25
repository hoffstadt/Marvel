#include "common/cbuffers.hlsli"
#include "common/equations.hlsli"

// textures
Texture2D   ColorTexture    : register(t0);
Texture2D   SpecularTexture : register(t1);
Texture2D   NormalTexture   : register(t2);
TextureCube ShadowMap      : register(t3);

// samplers
SamplerState           Sampler       : register(s0);
SamplerComparisonState ShadowSampler : register(s1);

// constant buffers
cbuffer mvPointLightCBuf       : register(b0) { mvPointLight PointLight; };
cbuffer mvMaterialCBuf         : register(b1) { mvMaterial material; };
cbuffer mvDirectionalLightCBuf : register(b2) { mvDirectionalLight DirectionalLight; };
cbuffer mvSceneCBuf            : register(b3) { mvScene Scene; };

float4 main(float3 viewFragPos : Position, float3 viewNormal : Normal, float3 viewTan : Tangent, float3 viewBitan : Bitangent, 
    float2 tc : Texcoord, float4 spos : shadowPosition) : SV_Target
{
    float3 diffuse = { 0.0f, 0.0f, 0.0f };
    float3 specularReflected = { 0.0f, 0.0f, 0.0f };
    float3 specularReflectedColor = material.specularColor;
    float3 materialColor = material.materialColor;
    
    if(material.useTextureMap)
    {
        // sample diffuse texture
        const float4 dtex = ColorTexture.Sample(Sampler, tc);
        
        materialColor = dtex.rgb;
    }

    if (material.hasAlpha)
    {

        // sample diffuse texture
        const float4 dtex = ColorTexture.Sample(Sampler, tc);
            
        // bail if highly translucent
        clip(dtex.a < 0.1f ? -1 : 1);
        
        // flip normal when backface
        if (dot(viewNormal, viewFragPos) >= 0.0f)
        {
            viewNormal = -viewNormal;
        }
    }

    // normalize the mesh normal
    viewNormal = normalize(viewNormal);
    
    // replace normal with mapped if normal mapping enabled
    if (material.useNormalMap)
    {
        const float3 mappedNormal = MapNormal(normalize(viewTan), normalize(viewBitan), viewNormal, tc, NormalTexture, Sampler);
        viewNormal = lerp(viewNormal, mappedNormal, material.normalMapWeight);
    }
    
    // specular parameter determination (mapped or uniform)
    float specularPowerLoaded = material.specularGloss;
    
    if (material.useSpecularMap)
    {
        const float4 specularSample = SpecularTexture.Sample(Sampler, tc);
        specularReflectedColor = specularSample.rgb;
        
        if (material.useGlossAlpha)
        {
            specularPowerLoaded = pow(2.0f, specularSample.a * 13.0f);
        }
    }
     
    // point light
    const float shadowLevel = Shadow(spos, ShadowMap, ShadowSampler);
            
    if (shadowLevel != 0.0f)
    {
            
	    // fragment to light vector data
        const LightVectorData lv = CalculateLightVectorData(PointLight.viewLightPos, viewFragPos);
    
	    // attenuation
        const float att = Attenuate(PointLight.attConst, PointLight.attLin, PointLight.attQuad, lv.dist);
    
	    // diffuse
        diffuse += Diffuse(PointLight.diffuseColor, PointLight.diffuseIntensity, att, lv.dir, viewNormal);
    
        // specular
        specularReflected += Speculate(
        PointLight.diffuseColor * PointLight.diffuseIntensity * specularReflectedColor, 1.0f, viewNormal,
        lv.vec, viewFragPos, att, specularPowerLoaded);
            
        // scale by shadow level
        diffuse *= shadowLevel;
        specularReflected *= shadowLevel;
    }
  
    
    // directional light

	// diffuse
    diffuse += Diffuse(DirectionalLight.diffuseColor, DirectionalLight.diffuseIntensity, 1.0f, -normalize(DirectionalLight.viewLightDir), viewNormal);
    
    // specular
    specularReflected += Speculate(
        DirectionalLight.diffuseColor * DirectionalLight.diffuseIntensity * specularReflectedColor, 1.0f, viewNormal,
        -normalize(DirectionalLight.viewLightDir), viewFragPos, 1.0f, specularPowerLoaded);

	// final color
    float3 litColor = saturate((diffuse + Scene.ambient) * materialColor + specularReflected);   
    return float4(litColor, 1.0f);
    //return float4(Fog(distance(float3(0.0f, 0.0f, 0.0f), viewFragPos), Scene.FogStart, Scene.FogRange, Scene.FogColor, litColor), 1.0f);

}