Texture2D t1 : register(t0);
Texture2D t2 : register(t1);
Texture2D t3 : register (t2);
SamplerState s1 : register(s0);

struct VS_OUTPUT
{
    float4 pos: SV_POSITION;
    float3 nor: NORMAL;
    float2 texCoord: TEXCOORD;
    float3 tan: TANGENT;
    float3 bitan : BITANGENT;
    float4 wPos : WORLDPOS;
    float3 eyeVec : EYEVEC;
};

cbuffer ConstantBuffer : register(b1)
{
    float4 ambientColor;
    float glossiness;
    float3 pad;
};

cbuffer ConstantBuffer : register(b2)
{
    float4 lightColor1;
    float3 lightPos1;
    float decayScale1;
};

float4 main(VS_OUTPUT input) : SV_TARGET
{
    float4 light1 = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 textureColor = t1.Sample(s1, input.texCoord);
    float3 bumpMap = t2.Sample(s1, input.texCoord) * 2.0f - 1;
    float4 specTex = t3.Sample(s1, input.texCoord);

    // Calculate the normal from the data in the bump map.
    float3 bumpNormal = (bumpMap.x * -input.tan) + (bumpMap.y * input.bitan) + (bumpMap.z * input.nor);
    bumpNormal = normalize(bumpNormal);

    //Necessary vectors.
    float3 lightDir1 = normalize(lightPos1 - input.wPos.xyz);

    //Ambient.
    float4 ambient = ambientColor * textureColor;

    //Diffuse computations.
    //float lightIntensity1 = max(dot(bumpNormal, lightDir1), 0.0f);
    float lightIntensity1 = saturate(dot(bumpNormal, lightDir1));
    if (lightIntensity1 > 0.0f) {

        float4 diffuse1 = saturate(lightIntensity1 * lightColor1) * textureColor;

        //Specular computations.
        float3 reflection1 = normalize(2 * lightIntensity1 * bumpNormal - lightDir1);
        //float3 reflection1 = reflect(-lightDir1, bumpNormal);
        //float3 halfAngle1 = normalize(lightDir1 + input.eyeVec);
        //float clampedNormalDotHalfAngle1 = saturate(dot(bumpNormal, halfAngle1));
        //float gloss = glossiness * specTex.a;
        float4 specular1 = pow(saturate(dot(reflection1, input.eyeVec)), glossiness);
        //float4 specular1 = pow(clampedNormalDotHalfAngle1, glossiness);
        specular1 = specular1 * specTex/* * lightColor1*/;
        //specular1 = specular1 * lightColor1;

        //Attenuation.
        float distance1 = length(lightPos1);
        //Linear light decay
        //float attenuation1 = saturate(1.0f / distance1) * decayScale1;
        float attenuation1 = (1.0f / distance1) * decayScale1;
        //float attenuation1 = 1.0f - /*saturate*/(distance1 / decayScale1);

        light1 = (diffuse1 + specular1) * attenuation1;
    }
    //Final color.
    return saturate(ambient + light1)/* * textureColor*/;
}