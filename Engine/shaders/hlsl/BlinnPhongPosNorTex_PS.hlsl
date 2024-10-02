cbuffer LightCBuf : register(b0)
{
    float3 lightPos;
    float3 ambient;
    float3 diffuseColor;
    float diffuseIntensity;
    float attConst;
    float attLin;
    float attQuad;
    float3 viewPos;
};

cbuffer ObjectCBuf : register(b1)
{
    float3 materialColor;
    float specularIntensity;
    float specularPower;
}

Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

struct VertexPosHWNormalTex
{
    float4 position : SV_POSITION;
    float3 worldPos : POSITION; // The position of the vertex in world space
    float3 normalW : NORMAL;    // The direction of the normal vector in the world
    float2 tex : TEXCOORD;
};

float4 PS(VertexPosHWNormalTex pIn) : SV_Target
{
    float4 color;
    
    // fragment to light vector data
    const float3 vToL = lightPos - pIn.worldPos;
    const float distToL = length(vToL);
    const float3 dirToL = normalize(vToL);
    // diffuse attenuation
    const float att = 1.0f / (attConst + attLin * distToL + attQuad * (distToL * distToL));
    // disffuse intensity
    const float3 diffuse = att * diffuseColor * diffuseIntensity * max(0.0f, dot(dirToL, pIn.normalW));

    // specular
    const float3 viewDir = normalize(viewPos - pIn.worldPos);
    const float3 halfwayDir = normalize(dirToL + viewDir);
    const float3 specular = att * (diffuseColor * diffuseIntensity) * specularIntensity * pow(max(0.0f, dot(pIn.normalW, halfwayDir)), specularPower);
    
    // final color
    color = float4(saturate((ambient + diffuse) * materialColor * shaderTexture.Sample(SampleType, pIn.tex).rgb + specular), 1.0f);
    
    //return color;
    return color;
}