#include "common/cbuffers.hlsli"
#include "common/equations.hlsli"

// Constant normal incidence Fresnel factor for all dielectrics.
static const float Epsilon = 0.00001;

//-----------------------------------------------------------------------------
// textures
//-----------------------------------------------------------------------------
Texture2D   AlbedoTexture     : register(t0);
Texture2D   NormalTexture     : register(t1);
Texture2D   RoughnessTexture  : register(t2);
TextureCube ShadowMap         : register(t3);
Texture2D   MetalTexture      : register(t4);
TextureCube IrradianceTexture : register(t5);

//-----------------------------------------------------------------------------
// samplers
//-----------------------------------------------------------------------------
SamplerState           Sampler       : register(s0);
SamplerComparisonState ShadowSampler : register(s1);
SamplerState           CubeSampler   : register(s2);

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
    float3 worldNormal : Normal1; // frag norm (view space)
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
    
        float3 Li = -DirectionalLight.viewLightDir;

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
    
    //-----------------------------------------------------------------------------
    // ambient lighting
    //-----------------------------------------------------------------------------
    float3 ambientLighting;
    {
        
        // Sample diffuse irradiance at normal direction.
        float3 irradiance = IrradianceTexture.Sample(CubeSampler, normalize(input.worldNormal)).rgb;

		// Calculate Fresnel term for ambient lighting.
		// Since we use pre-filtered cubemap(s) and irradiance is coming from many directions
		// use cosLo instead of angle with light's half-vector (cosLh above).
		// See: https://seblagarde.wordpress.com/2011/08/17/hello-world/
        //float3 F = fresnelSchlick(F0, cosLo);
        float3 F = fresnelSchlick(F0, cosLo);

		// Get diffuse contribution factor (as with direct lighting).
        float3 kd = lerp(1.0 - F, 0.0, metalness);

		// Irradiance map contains exitant radiance assuming Lambertian BRDF, no need to scale by 1/PI here either.
        float3 diffuseIBL = kd * albedo * irradiance;

		// Sample pre-filtered specular reflection environment at correct mipmap level.
        //uint specularTextureLevels = querySpecularTextureLevels();
        //float3 specularIrradiance = specularTexture.SampleLevel(defaultSampler, Lr, roughness * specularTextureLevels).rgb;

		// Split-sum approximation factors for Cook-Torrance specular BRDF.
        //float2 specularBRDF = specularBRDF_LUT.Sample(spBRDF_Sampler, float2(cosLo, roughness)).rg;

		// Total specular IBL contribution.
        //float3 specularIBL = (F0 * specularBRDF.x + specularBRDF.y) * specularIrradiance;

		// Total ambient lighting contribution.
        //ambientLighting = diffuseIBL + specularIBL;
        ambientLighting = diffuseIBL;
    }
    
    return float4(directLighting + ambientLighting, 1.0f);
}