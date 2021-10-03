Texture2D t1 : register(t0);
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

float4 main(VS_OUTPUT input) : SV_TARGET
{
    // return interpolated color
    return t1.Sample(s1, input.texCoord);
}