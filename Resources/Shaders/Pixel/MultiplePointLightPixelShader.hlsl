Texture2D t1 : register(t0);
Texture2D t2 : register(t1);
Texture2D t3 : register (t2);
SamplerState s1 : register(s0);

struct GS_OUTPUT
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
    float4 emissiveColor;
    float glossiness;
};

cbuffer ConstantBuffer : register(b2)
{
    struct ConstantbufferLight {

        float4 lightColor;
        float4 lightPos;
        float4 lightDir;
        float constant;
        float linearAtt;
        float quadratic;
        float cutOff;
    };

    ConstantbufferLight lights[8];
};

float4 ComputeDirLights(ConstantbufferLight light, float3 normals, float3 eyeVec, float4 textureColor, float4 specTex) {

    float3 lightDir = normalize(light.lightDir.xyz);

    //Diffuse computations.
    float lightIntensity = saturate(dot(normals, lightDir));
    if (lightIntensity > 0.0f) {

        float4 diffuse = saturate(lightIntensity * light.lightColor)/* * textureColor*/;

        //Specular computations.
        float3 reflection = normalize(2 * lightIntensity * normals - lightDir);
        float4 specular = pow(saturate(dot(reflection, eyeVec)), glossiness);
        specular = specular * specTex;

        return (diffuse + specular);
    }
    return float4(0.0f, 0.0f, 0.0f, 1.0f);
}

float4 ComputePointLights(ConstantbufferLight light, float3 normals, float3 pixPos, float3 eyeVec, float4 textureColor, float4 specTex) {

    float3 lightDir = normalize(light.lightPos.xyz - pixPos);

    //Diffuse computations.
    float lightIntensity = saturate(dot(normals, lightDir));
    if (lightIntensity > 0.0f) {

        float4 diffuse = saturate(lightIntensity * light.lightColor)/* * textureColor*/;

        //Specular computations.
        float3 reflection = normalize(2 * lightIntensity * normals - lightDir);
        float4 specular = pow(saturate(dot(reflection, eyeVec)), glossiness);
        specular = specular * specTex;

        //Attenuation.
        float distance = length(light.lightPos.xyz - pixPos);
        //Linear light decay
        //float attenuation = (1.0f / distance) * lights[i].decayScale;
        //float attenuation = saturate(1.0f / distance) * lights[i].decayScale;
        float attenuation = 1.0 / (light.constant + light.linearAtt * distance + light.quadratic * (distance * distance));

        return (diffuse + specular) * attenuation;
    }
    return float4(0.0f, 0.0f, 0.0f, 1.0f);
}

float4 ComputeSpotLights(ConstantbufferLight light, float3 normals, float3 pixPos, float3 eyeVec, float4 textureColor, float4 specTex) {

    float3 lightDir = normalize(light.lightPos.xyz - pixPos);

    //Diffuse computations.
    float lightIntensity = saturate(dot(normals, lightDir));
    if (lightIntensity > 0.0f) {

        float4 diffuse = saturate(lightIntensity * light.lightColor)/* * textureColor*/;

        //Specular computations.
        float3 reflection = normalize(2 * lightIntensity * normals - lightDir);
        float4 specular = pow(saturate(dot(reflection, eyeVec)), glossiness);
        specular = specular * specTex;

        //Attenuation.
        float distance = length(light.lightPos.xyz - pixPos);
        //Linear light decay
        //float attenuation = (1.0f / distance) * lights[i].decayScale;
        //float attenuation = saturate(1.0f / distance) * lights[i].decayScale;
        float attenuation = 1.0 / (light.constant + light.linearAtt * distance + light.quadratic * (distance * distance));
        float spotCone = pow(saturate(dot(lightDir, normalize(-light.lightDir))), light.cutOff);

        return (diffuse + specular) * spotCone * attenuation;
    }
    return float4(0.0f, 0.0f, 0.0f, 1.0f);
}

float4 main(GS_OUTPUT input) : SV_TARGET
{
    float4 light = float4(0.0f, 0.0f, 0.0f, 1.0f);
    float4 textureColor = t1.Sample(s1, input.texCoord);
    float3 bumpMap = t2.Sample(s1, input.texCoord) * 2.0f - 1;
    float4 specTex = t3.Sample(s1, input.texCoord);

    // Calculate the normal from the data in the bump map.
    float3 bumpNormal = (bumpMap.x * -input.tan) + (bumpMap.y * input.bitan) + (bumpMap.z * input.nor);
    bumpNormal = normalize(bumpNormal);

    //Ambient.
    float4 ambient = ambientColor * textureColor;

    [unroll(8)]
    for (unsigned int i = 0; i < 8; ++i) {
        if(lights[i].lightColor.w != 0.0f){
           if(lights[i].lightPos.w == 1.0f && lights[i].lightDir.w == 1.0f)
                light += ComputePointLights(lights[i], bumpNormal, input.wPos.xyz, input.eyeVec, textureColor, specTex);
            else if (lights[i].lightPos.w == 0.0f && lights[i].lightDir.w == 0.0f)
                light += ComputeDirLights(lights[i], bumpNormal, input.eyeVec, textureColor, specTex);
            else
                light += ComputeSpotLights(lights[i], bumpNormal, input.wPos.xyz, input.eyeVec, textureColor, specTex);
        }
    }
    //Final color.
    return saturate(ambient + emissiveColor + light) * textureColor;
}