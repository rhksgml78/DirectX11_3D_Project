#include "Common.hlsl"

struct VertexInput
{
	float4 Position : POSITION0;
	float2 Uv : UV0;
	float3 Normal : NORMAL0;
	float3 Tangent : TANGENT0;
	//정점변환때만 쓰이는 멤버
	float4 Indices : INDICES0;
	float4 Weights : WEIGHTS0;
};
struct PixelInput
{
	float4 Position : SV_POSITION;
	float3 wPosition : POSITION0;
	float2 Uv : UV0;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
	float3 Binormal : BINORMAL;
};

PixelInput VS(VertexInput input)
{
   
	PixelInput output;
    //output.Color = input.Color;
	output.Uv = input.Uv;
	
	Matrix world;
    [flatten]
	if (input.Weights.x)
		world = SkinWorld(input.Indices, input.Weights);
	else
		world = World;
		
	output.Position = mul(input.Position, world);
	
	
	
	output.wPosition = output.Position.xyz;
	output.Position = mul(output.Position, ViewProj);
	//input.Normal.w = 0.0f;
	output.Normal = mul(input.Normal, (float3x3) world);
	//input.Tangent.w = 0.0f;
	output.Tangent = mul(input.Tangent, (float3x3) world);
	output.Binormal = cross(output.Normal.xyz, output.Tangent.xyz);
    
	return output;
}

//픽셀쉐이더 부분이 다름
float4 PS(PixelInput input) : SV_TARGET
{
	//        rgb값으로 리턴이 아닌 z값으로 리턴
	float BaseColor = input.Position.z / input.Position.w;
    return float4(BaseColor, BaseColor, BaseColor, 1.0f);
    //float BaseColorR = (input.Position.z + colorr) / input.Position.w;
	//float BaseColorG = (input.Position.z + colorg) / input.Position.w ;
	//float BaseColorB = (input.Position.z + colorb) / input.Position.w ;
    //return float4(BaseColorR, BaseColorG, BaseColorB, 1.0f);
}