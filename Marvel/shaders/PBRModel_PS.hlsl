#include "common/cbuffers.hlsli"
#include "common/equations.hlsli"

// Constant normal incidence Fresnel factor for all dielectrics.
static const float Epsilon = 0.00001;

//-----------------------------------------------------------------------------
// textures
//-----------------------------------------------------------------------------
Texture2D AlbedoTexture    : register(t0);
Texture2D NormalTexture    : register(t1);
Texture2D RoughnessTexture : register(t2);
TextureCube ShadowMap      : register(t3);
Texture2D MetalTexture     : register(t4);

//-----------------------------------------------------------------------------
// samplers
//-----------------------------------------------------------------------------
SamplerState Sampler : register(s0);
SamplerComparisonState ShadowSampler : register(s1);

//-----------------------------------------------------------------------------
// constant buffers
//-----------------------------------------------------------------------------
cbuffer mvPointLightCBuf       : register(b0) { mvPointLight PointLight; };
cbuffer mvMaterialCBuf         : register(b1) { mvPBRMaterial material; };
cbuffer mvDirectionalLightCBuf : register(b2) { mvDirectionalLight DirectionalLight; };
cbuffer mvSceneCBuf            : register(b3) { mvScene Scene; };

struct VSOut
{
    float3 viewPos        : Position;       // frag pos  (view space)
    float3 viewNormal     : Normal;         // frag norm (view space)
    float2 tc             : Texcoord;       // texture coordinates
    float3x3 tangentBasis : TangentBasis;   // tangent basis
    float4 shadowWorldPos : shadowPosition; // light pos (world space)
    float4 pixelPos       : SV_Position;    // frag pos  (screen space)
};

float4 main(VSOut input) : SV_Target
{
    
    float3 albedo = material.albedo;
    float metalness = material.metalness;
    float roughness = material.roughness;
    
    if(material.useAlbedoMap)
    {
        albedo = AlbedoTexture.Sample(Sampler, input.tc).rgb;
    }
    
    if (material.useMetalMap)
    {
        metalness = MetalTexture.Sample(Sampler, input.tc).r;
    }
    
    if (material.useRoughnessMap)
    {
        roughness = RoughnessTexture.Sample(Sampler, input.tc).r;
    }
    
    // replace normal with mapped if normal mapping enabled
    if (material.useNormalMap)
    {
        // sample and unpack the normal from texture into target space   
        const float3 normalSample = NormalTexture.Sample(Sampler, input.tc).xyz;
        const float3 tanNormal = normalSample * 2.0f - 1.0f;
        const float3 mappedNormal = normalize(mul(tanNormal, input.tangentBasis));
        input.viewNormal = lerp(input.viewNormal, mappedNormal, 1.0f);
    }
  
    // outgoing light direction (fragment position to the "eye").
    const float3 Lo = -normalize(input.viewPos);
    
    // normalize the mesh normal
    input.viewNormal = normalize(input.viewNormal);
    
    // angle between surface normal and outgoing light direction
    float cosLo = max(0.0f, dot(input.viewNormal, Lo));
    
    // specular reflection vector
    float3 Lr = 2.0 * cosLo * input.viewNormal - Lo;
    
    // fresnel reflectance at normal incidence (for metals use albedo color).
    float3 F0 = lerp(material.fresnel, albedo, metalness);
    
    float3 directLighting = 0.0f;
    
    //-----------------------------------------------------------------------------
    // point light
    //-----------------------------------------------------------------------------
    float shadowLevel = Shadow(input.shadowWorldPos, ShadowMap, ShadowSampler);
    if (!Scene.useShadows)            
        shadowLevel = 1.0f;
    if (shadowLevel != 0.0f)
    {
    
        // fragment to light vector data
        float3 lightVec = PointLight.viewLightPos - input.viewPos;
        float lightDistFromFrag = length(lightVec);
        float3 lightDirVec = lightVec / lightDistFromFrag;
        
        // attenuation
        const float att = Attenuate(PointLight.attConst, PointLight.attLin, PointLight.attQuad, lightDistFromFrag);
    
        float3 Li = lightDirVec;

	    // Half-vector between Li and Lo.
	    float3 Lh = normalize(Li + Lo);

	    // Calculate angles between surface normal and light vector.
	    float cosLi = max(0.0, dot(input.viewNormal, Li));
	    float cosLh = max(0.0, dot(input.viewNormal, Lh));

	    // Calculate Fresnel term for direct lighting. 
	    float3 F  = fresnelSchlick(F0, max(0.0, dot(Lh, Lo)));
    
	    // Calculate normal distribution for specular BRDF.
	    float D = ndfGGX(cosLh, roughness);
    
	    // Calculate geometric attenuation for specular BRDF.
	    float G = gaSchlickGGX(cosLi, cosLo, roughness);

	    // Diffuse scattering happens due to light being refracted multiple times by a dielectric medium.
	    // Metals on the other hand either reflect or absorb energy, so diffuse contribution is always zero.
	    // To be energy conserving we must scale diffuse BRDF contribution based on Fresnel factor & metalness.
	    float3 kd = lerp(float3(1, 1, 1) - F, float3(0, 0, 0), metalness);

	    // Lambert diffuse BRDF.
	    // We don't scale by 1/PI for lighting & material units to be more convenient.
	    // See: https://seblagarde.wordpress.com/2012/01/08/pi-or-not-to-pi-in-game-lighting-equation/
        float3 diffuseBRDF = kd * albedo;

	    // Cook-Torrance specular microfacet BRDF.
	    float3 specularBRDF = (F * D * G) / max(Epsilon, 4.0 * cosLi * cosLo);

	    // Total contribution for this light.
        directLighting += (diffuseBRDF + specularBRDF) * PointLight.diffuseIntensity * att * cosLi;
        directLighting *= shadowLevel;
    }
    
    //-----------------------------------------------------------------------------
    // directional light
    //-----------------------------------------------------------------------------
    {
    
        float3 Li = DirectionalLight.viewLightDir;

	    // Half-vector between Li and Lo.
        float3 Lh = normalize(Li + Lo);

	    // Calculate angles between surface normal and light vector.
        float cosLi = max(0.0, dot(input.viewNormal, Li));
        float cosLh = max(0.0, dot(input.viewNormal, Lh));

	    // Calculate Fresnel term for direct lighting. 
        float3 F = fresnelSchlick(F0, max(0.0, dot(Lh, Lo)));
    
	    // Calculate normal distribution for specular BRDF.
        float D = ndfGGX(cosLh, roughness);
    
	    // Calculate geometric attenuation for specular BRDF.
        float G = gaSchlickGGX(cosLi, cosLo, roughness);

	    // Diffuse scattering happens due to light being refracted multiple times by a dielectric medium.
	    // Metals on the other hand either reflect or absorb energy, so diffuse contribution is always zero.
	    // To be energy conserving we must scale diffuse BRDF contribution based on Fresnel factor & metalness.
        float3 kd = lerp(float3(1, 1, 1) - F, float3(0, 0, 0), metalness);

	    // Lambert diffuse BRDF.
	    // We don't scale by 1/PI for lighting & material units to be more convenient.
	    // See: https://seblagarde.wordpress.com/2012/01/08/pi-or-not-to-pi-in-game-lighting-equation/
        float3 diffuseBRDF = kd * albedo;

	    // Cook-Torrance specular microfacet BRDF.
        float3 specularBRDF = (F * D * G) / max(Epsilon, 4.0 * cosLi * cosLo);

	    // Total contribution for this light.
        directLighting += (diffuseBRDF + specularBRDF) * DirectionalLight.diffuseIntensity * cosLi;
    }
    
    return float4(directLighting + Scene.ambient, 1.0f);
}