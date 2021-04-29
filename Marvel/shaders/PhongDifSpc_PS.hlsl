#include "operations.hlsli"
#include "pointlight.hlsli"
#include "transform.hlsli"
#include "directionallight.hlsli"

cbuffer ObjectCBuf : register(b1)
{
    bool useGlossAlpha;
    bool useSpecularMap;
    float3 specularColor;
    float specularWeight;
    float specularGloss;
};

Texture2D tex : register(t0);
Texture2D spec : register(t1);

SamplerState splr : register(s0);


float4 main(float3 viewFragPos : Position, float3 viewNormal : Normal, float2 tc : Texcoord) : SV_Target
{
    float3 diffuse;
    float3 specularReflected;
    
    // normalize the mesh normal
    viewNormal = normalize(viewNormal);
    
    // specular parameters
    float specularPowerLoaded = specularGloss;
    const float4 specularSample = spec.Sample(splr, tc);
    float3 specularReflectionColor;
    if (useSpecularMap)
    {
        specularReflectionColor = specularSample.rgb;
    }
    else
    {
        specularReflectionColor = specularColor;
    }
    if (useGlossAlpha)
    {
        specularPowerLoaded = pow(2.0f, specularSample.a * 13.0f);
    }
    
    
    for (int i = 0; i < LightCount; i++)
    {
    
	    // fragment to light vector data
        const LightVectorData lv = CalculateLightVectorData(viewLightPos[i], viewFragPos);
    
	    // attenuation
        const float att = Attenuate(attConst[i], attLin[i], attQuad[i], lv.dist);
    
	    // diffuse
        diffuse += Diffuse(diffuseColor[i], diffuseIntensity[i], att, lv.dir, viewNormal);
    
        // specular
        specularReflected += Speculate(
            diffuseColor[i] * diffuseIntensity[i] * specularReflectionColor, specularWeight, viewNormal,
            lv.vec, viewFragPos, att, specularPowerLoaded
        );
    }
    
    for (int i = 0; i < dLightCount; i++)
    {
        
	    // diffuse
        diffuse += Diffuse(ddiffuseColor[i], ddiffuseIntensity[i], 1.0f, -normalize(viewLightDir[i]), viewNormal);
    
        // specular
        specularReflected += Speculate(
            ddiffuseColor[i] * ddiffuseIntensity[i] * specularColor, specularWeight, viewNormal,
            -normalize(viewLightDir[i]), viewFragPos, 1.0f, specularGloss
        );
    }

	// final color = attenuate diffuse & ambient by diffuse texture color and add specular reflected
    return float4(saturate((diffuse + ambient) * tex.Sample(splr, tc).rgb + specularReflected), 1.0f);
}