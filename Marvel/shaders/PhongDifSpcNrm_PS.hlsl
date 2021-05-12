#include "common/operations.hlsli"
#include "common/pointlight.hlsli"
#include "common/directionallight.hlsli"
#include "common/pshadow.hlsli"

cbuffer ObjectCBuf : register(b1)
{
    bool useGlossAlpha;
    bool useSpecularMap;
    float3 specularColor;
    float specularWeight;
    float specularGloss;
    bool useNormalMap;
    float normalMapWeight;
};

Texture2D tex : register(t0);
Texture2D spec : register(t1);
Texture2D nmap : register(t2);

SamplerState splr : register(s0);


float4 main(float3 viewFragPos : Position, float3 viewNormal : Normal, float3 viewTan : Tangent, float3 viewBitan : Bitangent, float2 tc : Texcoord, float4 spos : shadowPosition) : SV_Target
{
    float3 diffuse;
    float3 specularReflected;
    
    // sample diffuse texture
    const float4 dtex = tex.Sample(splr, tc);
    
#ifdef MASK_BOI
    // bail if highly translucent
    clip(dtex.a < 0.1f ? -1 : 1);
    // flip normal when backface
    if (dot(viewNormal, viewFragPos) >= 0.0f)
    {
        viewNormal = -viewNormal;
    }
#endif
    
    // normalize the mesh normal
    viewNormal = normalize(viewNormal);
    
    // replace normal with mapped if normal mapping enabled
    if (useNormalMap)
    {
        const float3 mappedNormal = MapNormal(normalize(viewTan), normalize(viewBitan), viewNormal, tc, nmap, splr);
        viewNormal = lerp(viewNormal, mappedNormal, normalMapWeight);
    }
    
    // specular parameter determination (mapped or uniform)
    float3 specularReflectionColor;
    float specularPower = specularGloss;
    const float4 specularSample = spec.Sample(splr, tc);
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
        specularPower = pow(2.0f, specularSample.a * 13.0f);
    }
    
    
            // shadow map test
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
            diffuseColor[i] * diffuseIntensity[i] * specularReflectionColor, specularWeight, viewNormal,
            lv.vec, viewFragPos, att, specularPower
        );
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
            ddiffuseColor[i] * ddiffuseIntensity[i] * specularColor, specularWeight, viewNormal,
            -normalize(viewLightDir[i]), viewFragPos, 1.0f, specularGloss
        );
    }

	// final color = attenuate diffuse & ambient by diffuse texture color and add specular reflected
    
    float3 litColor = saturate((diffuse + ambient) * tex.Sample(splr, tc).rgb + specularReflected);
    
    return float4(Fog(distance(float3(0.0f, 0.0f, 0.0f), viewFragPos), FogStart, FogRange, FogColor, litColor), 1.0f);
}