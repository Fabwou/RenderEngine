
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

cbuffer ConstantBuffer : register(b3)
{
	float time;
	float magnitude;
	float2 pad;
};

float3 GetNormal(VS_OUTPUT input[3])
{
	float3 a = float3(input[0].pos.xyz) - float3(input[1].pos.xyz);
	float3 b = float3(input[2].pos.xyz) - float3(input[1].pos.xyz);
	return normalize(cross(a, b));
}

float4 explode(float4 position, float3 normal) {

	float3 direction = -normal * ((sin(time) + 1.0f) / 2.0f) * magnitude;
	return position + float4(direction, 0.0f);
}

[maxvertexcount(3)]
void main(
	triangle VS_OUTPUT input[3] : SV_POSITION,
	inout TriangleStream<GS_OUTPUT> output
)
{
	float3 test = GetNormal(input);
	GS_OUTPUT element;
	[unroll(3)]
	for (uint i = 0; i < 3; i++)
	{
		//element.pos = explode(input[i].pos, input[i].nor);
		element.pos = explode(input[i].pos, test);
		element.nor = input[i].nor;
		element.texCoord = input[i].texCoord;
		element.tan = input[i].tan;
		element.bitan = input[i].bitan;
		element.wPos = input[i].wPos;
		element.eyeVec = input[i].eyeVec;
		output.Append(element);
	}
	//output.RestartStrip();
	//[unroll(3)]
	//for (uint i = 0; i < 3; i++)
	//{
	//	element.pos = input[i].pos + float4(2.0f, 0.0f, 0.0f, 0.0f);
	//	element.nor = input[i].nor;
	//	element.texCoord = input[i].texCoord;
	//	element.tan = input[i].tan;
	//	element.bitan = input[i].bitan;
	//	element.wPos = input[i].wPos + float4(2.0f, 0.0f, 0.0f, 0.0f);
	//	element.eyeVec = input[i].eyeVec/*normalize(input[i].eyeVec + float4(2.0f, 0.0f, 0.0f, 0.0f))*/;
	//	output.Append(element);
	//}
}