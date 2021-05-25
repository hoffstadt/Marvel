
static const float PI = 3.14159265359;

//-----------------------------------------------------------------------------
// lighting
//-----------------------------------------------------------------------------
float Attenuate(uniform float attConst, uniform float attLin, uniform float attQuad, const in float distFragToL)
{
    return 1.0f / (attConst + attLin * distFragToL + attQuad * (distFragToL * distFragToL));
}

float3 Diffuse(uniform float3 diffuseColor, uniform float diffuseIntensity, const in float att,
    const in float3 viewDirFragToL, const in float3 viewNormal)
{
    return diffuseColor * diffuseIntensity * att * max(0.0f, dot(viewDirFragToL, viewNormal));
}

float3 Speculate(const in float3 specularColor, uniform float specularIntensity, const in float3 viewNormal,
    const in float3 viewFragToL, const in float3 viewPos, const in float att, const in float specularPower)
{
    // calculate reflected light vector
    const float3 w = viewNormal * dot(viewFragToL, viewNormal);
    const float3 r = normalize(w * 2.0f - viewFragToL);
    
    // vector from camera to fragment (in view space)
    const float3 viewCamToFrag = normalize(viewPos);
    
    // calculate specular component color based on angle between
    // viewing vector and reflection vector, narrow with power function
    return att * specularColor * specularIntensity * pow(max(0.0f, dot(-r, viewCamToFrag)), specularPower);
}

struct LightVectorData
{
    float3 vec;
    float3 dir;
    float dist;
};

LightVectorData CalculateLightVectorData(const in float3 lightPos, const in float3 fragPos)
{
    LightVectorData lv;
    lv.vec = lightPos - fragPos;
    lv.dist = length(lv.vec);
    lv.dir = lv.vec / lv.dist;
    return lv;
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
// misc
//-----------------------------------------------------------------------------
float3 MapNormal(const in float3 tan, const in float3 bitan, const in float3 normal,
    const in float2 tc, uniform Texture2D nmap, uniform SamplerState splr)
{
    // build the tranform (rotation) into same space as tan/bitan/normal (target space)
    const float3x3 tanToTarget = float3x3(tan, bitan, normal);
    // sample and unpack the normal from texture into target space   
    const float3 normalSample = nmap.Sample(splr, tc).xyz;
    const float3 tanNormal = normalSample * 2.0f - 1.0f;
    // bring normal from tanspace into target space
    return normalize(mul(tanNormal, tanToTarget));
}
