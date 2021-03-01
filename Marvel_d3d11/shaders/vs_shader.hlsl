
float4 main(float2 pos : Position) : SV_POSITION
{

    float4 position = float4(pos, 0.0f, 1.0f);

    return position;
}
