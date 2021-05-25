#include "common/cbuffers.hlsli"
#include "common/equations.hlsli"
#include "common/Phong.hlsli"

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

float4 main(VSOut input) : SV_Target
{

    float3 diffuse = { 0.0f, 0.0f, 0.0f };
    float3 specularReflected = { 0.0f, 0.0f, 0.0f };
    float3 specularReflectedColor = material.specularColor;
    float3 materialColor = material.materialColor;

    // normalize the mesh normal
    input.viewNormal = normalize(input.viewNormal);
    
    // specular parameter determination (mapped or uniform)
    float specularPowerLoaded = material.specularGloss;
    
    // point light          
	// fragment to light vector data
    const LightVectorData lv = CalculateLightVectorData(PointLight.viewLightPos, input.viewPos);
    
	// attenuation
    const float att = Attenuate(PointLight.attConst, PointLight.attLin, PointLight.attQuad, lv.dist);
    
	// diffuse
    diffuse += Diffuse(PointLight.diffuseColor, PointLight.diffuseIntensity, att, lv.dir, input.viewNormal);
    
    // specular
    specularReflected += Speculate(
    PointLight.diffuseColor * PointLight.diffuseIntensity * specularReflectedColor, 1.0f, input.viewNormal,
    lv.vec, input.viewPos, att, specularPowerLoaded);
            
    // directional light

	// diffuse
    diffuse += Diffuse(DirectionalLight.diffuseColor, DirectionalLight.diffuseIntensity, 1.0f, -normalize(DirectionalLight.viewLightDir), input.viewNormal);
    
    // specular
    specularReflected += Speculate(
        DirectionalLight.diffuseColor * DirectionalLight.diffuseIntensity * specularReflectedColor, 1.0f, input.viewNormal,
        -normalize(DirectionalLight.viewLightDir), input.viewPos, 1.0f, specularPowerLoaded);

	// final color
    float3 litColor = saturate((diffuse + Scene.ambient) * materialColor + specularReflected);
    return float4(litColor, 1.0f);
}