#include "operations.hlsli"
#include "pointlight.hlsli"
#include "directionallight.hlsli"
#include "pshadow.hlsli"

Texture2D tex : register(t0);
SamplerState splr : register(s0);

cbuffer ObjectCBuf : register(b1)
{
    float3 materialColor;
    float3 specularColor;
    float specularWeight;
    float specularGloss;
};


float4 main(float3 viewFragPos : Position, float3 viewNormal : Normal, float2 tc : Texcoord, float4 spos : shadowPosition) : SV_Target
{
    float3 diffuse;
    float3 specular;
    
    // normalize the mesh normal
    viewNormal = normalize(viewNormal);
    
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
            specular += Speculate(
            diffuseColor[i] * diffuseIntensity[i] * specularColor, specularWeight, viewNormal,
            lv.vec, viewFragPos, att, specularGloss
        );
            // scale by shadow level
            diffuse *= shadowLevel;
            specular *= shadowLevel;
        }
    
        //for (int i = 0; i < dLightCount; i++)
        //{
        
	       // // diffuse
        //    diffuse += Diffuse(ddiffuseColor[i], ddiffuseIntensity[i], 1.0f, -normalize(viewLightDir[i]), viewNormal);
    
        //    // specular
        //    specular += Speculate(
        //        ddiffuseColor[i] * ddiffuseIntensity[i] * specularColor, specularWeight, viewNormal,
        //        -normalize(viewLightDir[i]), viewFragPos, 1.0f, specularGloss
        //    );
        //}
    }
    else
    {
        diffuse = specular = 0.0f;
    }
    
	// final color
    return float4(saturate((diffuse + ambient) * tex.Sample(splr, tc).rgb + specular), 1.0f);
}