#include "pointlight.hlsli"
#include "operations.hlsli"

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

float4 main(float3 viewFragPos : Position, float3 viewNormal : Normal, float3 viewTan : Tangent, float3 viewBitan : Bitangent,
float2 tc : Texcoord) : SV_Target
{
    
    float3 diffuse;
    float3 specularReflected;
    
    // alpha test
    float4 dtex = tex.Sample(splr, tc);
    clip(dtex.a < 0.1f ? -1 : 1);
    
    // flip normal if backface
    if(dot(viewNormal, viewFragPos) >= 0.0f)
        viewNormal = -viewNormal;
    
    // normalize the mesh normal
    viewNormal = normalize(viewNormal);
  
    if (useNormalMap)
    {
        const float3 mappedNormal = MapNormal(normalize(viewTan), normalize(viewBitan), viewNormal, tc, nmap, splr);
        viewNormal = lerp(viewNormal, mappedNormal, normalMapWeight);
    }
    
	// fragment to light vector data
    const LightVectorData lv = CalculateLightVectorData(viewLightPos, viewFragPos);
    
    // specular parameter determination (mapped or uniform)
    float3 specularReflectionColor;
    float specularPower = specularGloss;
    const float4 specularSample = spec.Sample(splr, tc);
    if (useSpecularMap)
        specularReflectionColor = specularSample.rgb;
    else
        specularReflectionColor = specularColor;
    
    if (useGlossAlpha)
        specularPower = pow(2.0f, specularSample.a * 13.0f);
    
	// attenuation
    const float att = Attenuate(attConst, attLin, attQuad, lv.distToL);
    
	// diffuse
    diffuse = Diffuse(diffuseColor, diffuseIntensity, att, lv.dirToL, viewNormal);
    
    // specular
    specularReflected = Speculate(
        diffuseColor * diffuseIntensity * specularReflectionColor, specularWeight, viewNormal,
        lv.vToL, viewFragPos, att, specularPower);

	// final color
    return float4(saturate((diffuse + ambient) * dtex.rgb + specularReflected), dtex.a);
}