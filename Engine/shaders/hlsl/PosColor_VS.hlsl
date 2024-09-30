cbuffer CBuf : register(b0)
{
    matrix model;
    matrix view;
    matrix proj;
};

float4 VS(float3 position : POSITION) : SV_Position
{
    matrix viewProj = mul(view, proj);
    matrix mvp = mul(model, viewProj);
    
    return mul(float4(position, 1.0), mvp);
}