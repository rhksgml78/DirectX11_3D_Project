#include "Common.hlsl"
// 터레인에서 본인의 굴곡에 그림자 표현하는 쉐이더

struct VertexInput
{
    float4 Position : POSITION0;
    float2 Uv : UV0;
	float3 Normal : NORMAL0;
	float4 Weights : WEIGHTS0;
};
struct PixelInput
{
    float4 Position : SV_POSITION;
	float3 Normal : NORMAL0;
	float3 wPosition : POSITION0;
    float2 Uv : UV0;
	float4 Weights : WEIGHTS0;
	float4 vPosition : POSITION1;
};

PixelInput VS(VertexInput input)
{
   
    PixelInput output;
    output.Uv = input.Uv;
    output.Position = mul(input.Position, World);
	output.vPosition = mul(output.Position, ShadowVP);
	output.wPosition = output.Position.xyz;
    output.Position = mul(output.Position, ViewProj);
	output.Normal = mul(input.Normal, (float3x3) World);
	output.Weights = input.Weights;
	
    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
	// 색상 정해주는 코드들은 모두지웠다!
	float BaseColor = input.Position.z / input.Position.w;
	
	return float4(BaseColor, BaseColor, BaseColor, 1.0f);
}