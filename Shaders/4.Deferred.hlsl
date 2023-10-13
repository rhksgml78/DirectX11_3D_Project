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

struct DeferredOutput
{
    float4 diffuse : SV_TARGET0;
    float4 specular : SV_TARGET1;
    float4 normal : SV_TARGET2;
    float4 wPosition : SV_TARGET3;
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

DeferredOutput PS(PixelInput input)
{
    DeferredOutput output;
	output.diffuse = DiffuseMapping(input.Uv);
	output.normal = float4(NormalMapping(input.Normal, input.Tangent, input.Binormal, input.Uv), 0);
	EmissiveMapping(output.diffuse.rgb, input.Uv, input.Normal, input.wPosition);
	output.diffuse.rgb += Ka.rgb * output.diffuse.rgb;
    
    output.specular = float4(SpecularMapping(input.Uv), 0);
	output.wPosition = float4(input.wPosition, 1);
    return output;
}