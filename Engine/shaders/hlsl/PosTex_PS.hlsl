Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

struct PixelInputType
{
    float4 position : SV_Position;
    float2 tex : TEXCOORD0;
};

float4 PS(PixelInputType input) : SV_Target
{
    float4 color;
    
    color = shaderTexture.Sample(SampleType, input.tex);
    
    return color;
}