#include "Common.hlsl"

struct VertexInput
{
    float4 Position : POSITION0;
    float4 Color : COLOR0;
	float3 Normal : NORMAL0;
    matrix transform : INSTANCE_TRANSFORM;
};
struct PixelInput
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR0;
	float3 Normal : NORMAL0;
	float3 wPosition : POSITION0;
	float4 vPosition : POSITION1;
};

PixelInput VS(VertexInput input)
{
   
    PixelInput output;
    output.Color = input.Color;
	//output.Color.a = 1.0f;
	output.Position = mul(input.Position, input.transform); //내위치
	output.Position = mul(output.Position, World);  //부모위치
	output.vPosition = mul(output.Position, ShadowVP);
	output.wPosition = output.Position.xyz;
    output.Position = mul(output.Position, ViewProj);
	input.Normal.w = 0.0f;
	output.Normal = mul(input.Normal, (float3x3) World);
    
    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
	float4 BaseColor = input.Color;
	float3 Normal = normalize(input.Normal);
	BaseColor = Lighting(BaseColor, float2(0, 0), Normal, input.wPosition);
	BaseColor = AddShadow(BaseColor, input.vPosition);
	return BaseColor;
}