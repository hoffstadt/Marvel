#include "pointlight.hlsli"

float4 main(float3 viewPos : Position, float3 viewNormal : Normal) : SV_Target
{
    
    float3 diffuse;
    float3 specular;
    
    float3 materialColor = { 1.0f, 0.0f, 0.0f };
    float3 specularColor = { 1.0f, 1.0f, 1.0f };
    float specularWeight = 1.0f;
    float specularGloss = 15.0f;
    
    // normalize the mesh normal
    viewNormal = normalize(viewNormal);
	// fragment to light vector data
    const LightVectorData lv = CalculateLightVectorData(viewLightPos, viewPos);
	// attenuation
    const float att = Attenuate(attConst, attLin, attQuad, lv.distToL);
	// diffuse
    diffuse = Diffuse(diffuseColor, diffuseIntensity, att, lv.dirToL, viewNormal);
    // specular
    specular = Speculate(
        diffuseColor * diffuseIntensity * specularColor, specularWeight, viewNormal,
        lv.vToL, viewPos, att, specularGloss
    );

	// final color
    return float4(saturate((diffuse + ambient) * materialColor + specular), 1.0f);
}