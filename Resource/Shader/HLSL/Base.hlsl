cbuffer cbPerObject : register(b0)
{
    float4x4 worldViewProj;
}

struct VertexIn
{
    float3 PosL : POSITION;
    float4 Color : Color;
};
struct VertexOut
{
    float3 PosH : SV_POSITION;
    float4 Color : Color;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;

    vout.PosH = mul(float4(vin.PosL, 1.0f), worldViewProj);
    vout.Color = vin.Color;

    return vout;
}

float PS(VertexOut pin) : SV_Target
{
    return pin.Color;
}