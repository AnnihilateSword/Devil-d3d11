cbuffer LightCBuf : register(b0)
{
    float3 lightPos;
    float3 ambient;
    float3 diffuseColor;
    float diffuseIntensity;
    float attConst;
    float attLin;
    float attQuad;
};

cbuffer ObjectCBuf : register(b1)
{
    float3 materialColor;
}

Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

struct VertexPosHWNormalTex
{
    float4 position : SV_POSITION;
    float3 worldPos : POSITION; // 在世界中的位置
    float3 normalW : NORMAL; // 法向量在世界中的方向
    float2 tex : TEXCOORD;
};

float4 PS(VertexPosHWNormalTex pIn) : SV_Target
{
    float4 color;
    
    // fragment to light vector data
    const float3 vToL = lightPos - pIn.worldPos;
    const float distToL = length(vToL);
    const float3 dirToL = vToL / distToL;
    // diffuse attenuation
    const float att = 1.0f / (attConst + attLin * distToL + attQuad * (distToL * distToL));
    // disffuse intensity
    const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToL, pIn.normalW));

    // tex color
    float4 texColor = shaderTexture.Sample(SampleType, pIn.tex);
    
    // final color
    color = float4(saturate((diffuse + ambient) * materialColor), 1.0f) * texColor;
    
    //return color;
    return color;
}