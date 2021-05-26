
static const float PI = 3.14159265359;

//-----------------------------------------------------------------------------
// lighting
//-----------------------------------------------------------------------------
float Attenuate(uniform float attConst, uniform float attLin, uniform float attQuad, const in float distFragToL)
{
    return 1.0f / (attConst + attLin * distFragToL + attQuad * (distFragToL * distFragToL));
}

//-----------------------------------------------------------------------------
// shadows
//-----------------------------------------------------------------------------
static const float zf = 100.0f;
static const float zn = 0.5f;
static const float c1 = (zf + zn) / (zf - zn);
static const float c0 = -(2 * zn * zf) / (zf - zn);

float CalculateShadowDepth(const in float4 shadowPos)
{
    // get magnitudes for each basis component
    const float3 m = abs(shadowPos).xyz;
    // get the length in the dominant axis
    // (this correlates with shadow map face and derives comparison depth)
    const float major = max(m.x, max(m.y, m.z));
    // converting from distance in shadow light space to projected depth
    return (c1 * major + c0) / major;
}

float Shadow(const in float4 shadowPos, uniform TextureCube map, uniform SamplerComparisonState smplr)
{
    return map.SampleCmpLevelZero(smplr, shadowPos.xyz, CalculateShadowDepth(shadowPos));
}

//-----------------------------------------------------------------------------
// fog
//-----------------------------------------------------------------------------
float3 Fog(const in float dist, uniform float fogstart, uniform float fogrange, uniform float3 fogcolor, const in float3 litcolor)
{
    float s = saturate((dist - fogstart) / fogrange);
    return litcolor + s * (fogcolor - litcolor);
}

//-----------------------------------------------------------------------------
// pbr
//-----------------------------------------------------------------------------

// GGX/Towbridge-Reitz normal distribution function.
// Uses Disney's reparametrization of alpha = roughness^2.
float ndfGGX(float cosLh, float roughness)
{
    float alpha = roughness * roughness;
    float alphaSq = alpha * alpha;

    float denom = (cosLh * cosLh) * (alphaSq - 1.0) + 1.0;
    return alphaSq / (PI * denom * denom);
}

// Single term for separable Schlick-GGX below.
float gaSchlickG1(float cosTheta, float k)
{
    return cosTheta / (cosTheta * (1.0 - k) + k);
}

// Schlick-GGX approximation of geometric attenuation function using Smith's method.
float gaSchlickGGX(float cosLi, float cosLo, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0; // Epic suggests using this roughness remapping for analytic lights.
    return gaSchlickG1(cosLi, k) * gaSchlickG1(cosLo, k);
}

// Shlick's approximation of the Fresnel factor.
float3 fresnelSchlick(float3 F0, float cosTheta)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}