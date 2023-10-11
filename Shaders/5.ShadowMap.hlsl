#include "Common.hlsl"
// �ͷ��ο��� ������ ��� �׸��� ǥ���ϴ� ���̴�

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
	// ���� �����ִ� �ڵ���� ���������!
	float BaseColor = input.Position.z / input.Position.w;
	
	return float4(BaseColor, BaseColor, BaseColor, 1.0f);
}