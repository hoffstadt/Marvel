#include "transform.hlsli"
#include "pointlight.hlsli"

struct GeoOut
{
    float4 pos : SV_Position;
    float4 col : Color;
};

struct VS_Output
{
    float4 viewPos : Position;
    float4 viewNormal : Normal;
    float4 pos : SV_Position;
};

[maxvertexcount(3)]
void main(triangle VS_Output gin[3], inout TriangleStream<GeoOut> triStream)
{
    
    float3 diffuse;
    float3 specular;
    
    // normalize the mesh normal
    gin[0].viewNormal = normalize(gin[0].viewNormal);
	// fragment to light vector data
    const LightVectorData lv = CalculateLightVectorData(viewLightPos, gin[0].viewPos);
	// attenuation
    const float att = Attenuate(attConst, attLin, attQuad, lv.distToL);
	// diffuse
    diffuse = Diffuse(diffuseColor, diffuseIntensity, att, lv.dirToL, gin[0].viewNormal);
    // specular
    specular = Speculate(
        diffuseColor * diffuseIntensity * specularColor, specularWeight, gin[0].viewNormal,
        lv.vToL, gin[0].viewPos, att, specularGloss
    );

	// final colo
    
    //float4 color = float4(saturate((diffuse + ambient) * materialColor + specular), 1.0f);
    float4 color = float4(saturate((diffuse + ambient) * materialColor), 1.0f);

    //
    // Transform quad vertices to world space and output 
    // them as a triangle strip.
    //
    GeoOut gout;
    [unroll]
    for (int i = 0; i < 3; ++i)
    {
        gout.pos = gin[i].pos;
        gout.col = color;
        
        triStream.Append(gout);
    }
}