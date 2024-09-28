cbuffer CBuf : register(b0)
{
    matrix transform;
};

struct VertexInputType
{
    float3 position : POSITION;
    float2 tex : TEXCOORD0;
};

struct PixelInputType
{
    float4 position : SV_Position;
    float2 tex : TEXCOORD0;
};

PixelInputType VS(VertexInputType input)
{
    PixelInputType output;
    
    output.position = mul(float4(input.position, 1.0), transform);
    output.tex = input.tex;
    
    return output;
}