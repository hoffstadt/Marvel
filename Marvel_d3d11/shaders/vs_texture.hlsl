
struct VS_Output
{   
    float2 uv : Texcoord;  
    float4 pos : SV_POSITION;
};

VS_Output main(float2 pos : Position, float2 uv : Texcoord)
{
    VS_Output output;
    output.pos = float4(pos, 0.0f, 1.0f);
    output.uv = uv;
    return output;
}