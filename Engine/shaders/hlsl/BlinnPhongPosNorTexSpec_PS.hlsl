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

Texture2D diffuseTexture : register(t0);
Texture2D specularTexture : register(t1);
SamplerState SampleType : register(s0);


struct VertexPosHWNormalTex
{
    float4 position : SV_POSITION;
    float3 worldPos : POSITION; // The position of the vertex in world space
    float3 normalW : NORMAL; // The direction of the normal vector in the world
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
    const float4 sepcularSample = specularTexture.Sample(SampleType, pIn.tex);
    const float3 specularReflectionColor = sepcularSample.rgb;
    // The alpha value obtained from the specular map is between 0 and 1, 
    // which is too small a power, usually we would need thousands
    const float specularPower = pow(2.0f, sepcularSample.a * 13.0f);
    const float3 specular = att * (diffuseColor * diffuseIntensity) * pow(max(0.0f, dot(pIn.normalW, halfwayDir)), specularPower);
    
    // final color
    color = float4(saturate(ambient + diffuse) * diffuseTexture.Sample(SampleType, pIn.tex).rgb + specular * specularReflectionColor, 1.0f);
    
    //return color;
    return color;
}