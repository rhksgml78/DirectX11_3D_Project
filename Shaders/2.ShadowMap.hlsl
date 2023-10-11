#include "Common.hlsl"

struct VertexInput
{
    float4 Position : POSITION0;
    float4 Color : COLOR0;
	float3 Normal : NORMAL0;
};
struct PixelInput
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR0;
	float3 Normal : NORMAL0;
	float3 wPosition : POSITION0;
};

PixelInput VS(VertexInput input)
{
   
    PixelInput output;
    output.Color = input.Color;
	//output.Color.a = 1.0f;
    //  o           =  i X W
    output.Position = mul(input.Position, World);
	output.wPosition = output.Position.xyz;
    output.Position = mul(output.Position, ViewProj);
	output.Normal = mul(input.Normal, (float3x3) World);
    
    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
	float BaseColor = input.Position.z / input.Position.w;
    
	return float4(BaseColor, BaseColor, BaseColor, 1.0f);
}