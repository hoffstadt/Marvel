struct VS_Output
{
    float4 position : SV_POSITION;
    float2 TexCoord : TexCoord;
};

// textures
Texture2D ColorTexture : register(t0);

// samplers
SamplerState Sampler : register(s0);

// constant buffer
cbuffer ConstantBuffer : register(b0)
{
    float x_offset;
    float y_offset;
};

VS_Output vs_main(float2 pos : Position, float2 TexCoord : TexCoord)
{
    VS_Output output;
    output.position = float4(pos.x + x_offset, pos.y + y_offset, 0.0f, 1.0f);
    output.TexCoord = TexCoord;

    return output;
}

float4 ps_main(VS_Output input) : SV_TARGET
{
    return ColorTexture.Sample(Sampler, input.TexCoord);
}