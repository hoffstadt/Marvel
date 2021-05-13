#include "common/operations.hlsli"
#include "common/pointlight.hlsli"
#include "common/pshadow.hlsli"
#include "common/directionallight.hlsli"

cbuffer ObjectCBuf : register(b1)
{
    float3 materialColor;
    float3 specularColor;
    float specularWeight;
    float specularGloss;
    float normalMapWeight;
    
    bool useTextureMap;
    bool useNormalMap;
    bool useSpecularMap;
    bool useGlossAlpha;
    
};

Texture2D tex : register(t0);
Texture2D spec : register(t1);
Texture2D nmap : register(t2);

SamplerState splr : register(s0);

float4 main(float3 viewFragPos : Position, float3 viewNormal : Normal, float3 viewTan : Tangent, float3 viewBitan : Bitangent, float2 tc : Texcoord, float4 spos : shadowPosition) : SV_Target
{
    float3 diffuse = { 0.0f, 0.0f, 0.0f };
    float3 specularReflected;
    float3 specularReflectedColor = specularColor;
    
    if(useTextureMap)
    {
        
        // sample diffuse texture
        const float4 dtex = tex.Sample(splr, tc);
        
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
    if (useNormalMap)
    {
        const float3 mappedNormal = MapNormal(normalize(viewTan), normalize(viewBitan), viewNormal, tc, nmap, splr);
        viewNormal = lerp(viewNormal, mappedNormal, normalMapWeight);
    }
    
    // specular parameter determination (mapped or uniform)
    float specularPowerLoaded = specularGloss;
    
    if (useSpecularMap)
    {
        const float4 specularSample = spec.Sample(splr, tc);
        specularReflectedColor = specularSample.rgb;
        
        if (useGlossAlpha)
        {
            specularPowerLoaded = pow(2.0f, specularSample.a * 13.0f);
        }
    }
     
    const float shadowLevel = Shadow(spos);
    if (shadowLevel != 0.0f)
    {
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
                diffuseColor[i] * diffuseIntensity[i] * specularReflectedColor, specularWeight, viewNormal,
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
    
    for (int i = 0; i < dLightCount; i++)
    {
        
	    // diffuse
        diffuse += Diffuse(ddiffuseColor[i], ddiffuseIntensity[i], 1.0f, -normalize(viewLightDir[i]), viewNormal);
    
        // specular
        specularReflected += Speculate(
            ddiffuseColor[i] * ddiffuseIntensity[i] * specularReflectedColor, specularWeight, viewNormal,
            -normalize(viewLightDir[i]), viewFragPos, 1.0f, specularPowerLoaded);
    }

	// final color
    float3 litColor;
    if (useTextureMap)
    {
        litColor = saturate((diffuse + ambient) * tex.Sample(splr, tc).rgb + specularReflected);
    }
    else
    {
        litColor = saturate((diffuse + ambient) * materialColor + specularReflected);
    }
    
    return float4(Fog(distance(float3(0.0f, 0.0f, 0.0f), viewFragPos), FogStart, FogRange, FogColor, litColor), 1.0f);
}