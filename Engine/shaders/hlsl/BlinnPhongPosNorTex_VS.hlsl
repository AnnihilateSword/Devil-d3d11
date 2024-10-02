cbuffer CBuf : register(b0)
{
    matrix model;
    matrix view;
    matrix proj;
    matrix modelInvTranspose; // Used when transforming normal vectors
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
    float3 worldPos : POSITION; // The position of the vertex in world space
    float3 normalW : NORMAL;    // The direction of the normal vector in the world
    float2 tex : TEXCOORD;
};

VertexPosHWNormalTex VS(VertexPosNormalTex vIn)
{
    VertexPosHWNormalTex vOut;
    
    matrix viewProj = mul(view, proj);
    float4 posW = mul(float4(vIn.position, 1.0f), model);
    
    vOut.position = mul(posW, viewProj);
    vOut.worldPos = posW.xyz;
    vOut.normalW = normalize(mul(vIn.normal, (float3x3) modelInvTranspose));
    vOut.tex = vIn.tex;
    
    return vOut;
}