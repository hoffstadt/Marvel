#include "common/cbuffers.hlsli"
#include "common/equations.hlsli"

// textures
Texture2D   ColorTexture    : register(t0);
Texture2D   SpecularTexture : register(t1);
Texture2D   NormalTexture   : register(t2);
TextureCube ShadowMap       : register(t3);

// samplers
SamplerState           Sampler       : register(s0);
SamplerComparisonState ShadowSampler : register(s1);

// constant buffers
cbuffer mvPointLightManagerCBuf : register(b0) { mvPointLightManager PointLight; };
cbuffer mvMaterialCBuf          : register(b1) { mvMaterial material; };
cbuffer mvDirectionalLightCBuf  : register(b2) { mvDirectionalLightManager DirectionalLight; };
cbuffer mvSceneCBuf             : register(b3) { mvScene Scene; };

float4 main(float3 viewFragPos : Position, float3 viewNormal : Normal, float3 viewTan : Tangent, float3 viewBitan : Bitangent, 
    float2 tc : Texcoord, float4 spos : shadowPosition) : SV_Target
{
    float3 diffuse = { 0.0f, 0.0f, 0.0f };
    float3 specularReflected = { 0.0f, 0.0f, 0.0f };
    float3 specularReflectedColor = material.specularColor;
    float3 materialColor = materialColor;

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
        
        materialColor = dtex.rgb;
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
     
    const float shadowLevel = Shadow(spos, ShadowMap, ShadowSampler);
    if (shadowLevel != 0.0f)
    {
        for (int i = 0; i < PointLight.LightCount; i++)
        {
    
	        // fragment to light vector data
            const LightVectorData lv = CalculateLightVectorData(PointLight.viewLightPos[i], viewFragPos);
    
	        // attenuation
            const float att = Attenuate(PointLight.attConst[i], PointLight.attLin[i], PointLight.attQuad[i], lv.dist);
    
	        // diffuse
            diffuse += Diffuse(PointLight.diffuseColor[i], PointLight.diffuseIntensity[i], att, lv.dir, viewNormal);
    
            // specular
            specularReflected += Speculate(
                PointLight.diffuseColor[i] * PointLight.diffuseIntensity[i] * specularReflectedColor, material.specularWeight, viewNormal,
                lv.vec, viewFragPos, att, specularPowerLoaded);
            
        }
        
        // scale by shadow level
        diffuse *= shadowLevel;
        specularReflected *= shadowLevel;
    }
    else
    {
        diffuse = specularReflected = 0.0f;
    }
    
    for (int i = 0; i < 1; i++)
    {
        
	    // diffuse
        diffuse += Diffuse(DirectionalLight.diffuseColor[i], DirectionalLight.diffuseIntensity[i], 1.0f, -normalize(DirectionalLight.viewLightDir[i]), viewNormal);
    
        // specular
        specularReflected += Speculate(
            DirectionalLight.diffuseColor[i] * DirectionalLight.diffuseIntensity[i] * specularReflectedColor, material.specularWeight, viewNormal,
            -normalize(DirectionalLight.viewLightDir[i]), viewFragPos, 1.0f, specularPowerLoaded);
    }

	// final color
    float3 litColor = saturate((diffuse + Scene.ambient) * materialColor + specularReflected);   
    return float4(Fog(distance(float3(0.0f, 0.0f, 0.0f), viewFragPos), Scene.FogStart, Scene.FogRange, Scene.FogColor, litColor), 1.0f);

}