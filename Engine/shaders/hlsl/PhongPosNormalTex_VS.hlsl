cbuffer CBuf : register(b0)
{
    matrix model;
    matrix view;
    matrix proj;
};

struct VertexPosNormalTex
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD0;
};

struct VertexPosHWNormalTex
{
    float4 position : SV_POSITION;
    float3 worldPos : POSITION; // 在世界中的位置
    float3 normalW : NORMAL; // 法向量在世界中的方向
    float2 tex : TEXCOORD;
};

VertexPosHWNormalTex VS(VertexPosNormalTex vIn)
{
    VertexPosHWNormalTex vOut;
    
    float4 posW = mul(float4(vIn.position, 1.0f), model);
    matrix viewProj = mul(view, proj);
    
    vOut.position = mul(posW, viewProj);
    vOut.worldPos = posW.xyz;
    vOut.normalW = mul(vIn.normal, (float3x3) model);
    vOut.tex = vIn.tex;
    
    return vOut;
}