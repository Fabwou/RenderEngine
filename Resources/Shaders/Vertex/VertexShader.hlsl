struct VS_INPUT
{
    float3 pos : POSITION;
    float3 nor : NORMAL;
    float2 texCoord: TEXCOORD;
    float3 tan : TANGENT;
    float3 bitan : BITANGENT;
};

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

cbuffer ConstantBuffer : register(b0)
{
    float4x4 wMat;
    float4x4 vMat;
    float4x4 pMat;
    float4x4 mvp;
    float4x4 invWMat;
    float3 cameraPos;
    float pad;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    float4 newPosition = float4(input.pos, 1.0f);
    newPosition = mul(newPosition, mvp);
    output.pos = newPosition;

    //output.nor = normalize(mul(input.nor, wMat));
    output.nor = normalize(mul(input.nor, (float3x3)invWMat));
    output.texCoord = input.texCoord;
    output.tan = normalize(mul(input.tan, wMat));
    output.bitan = normalize(mul(input.bitan, wMat));

    float4 wPos = float4(input.pos, 1.0f);
    wPos = mul(wPos, wMat);
    output.wPos = wPos;

    output.eyeVec = normalize(cameraPos - wPos.xyz);

    return output;
}