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



struct GeometryInput
{
	float4 Position : SV_POSITION;
	float3 wPosition : POSITION0;
	float2 Uv : UV0;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
	float3 Binormal : BINORMAL;
	float Clip : SV_ClipDistance0;
};


struct PixelInput
{
	uint TargetIndex : SV_RenderTargetArrayIndex;
	float4 Position : SV_POSITION;
	float3 wPosition : POSITION0;
	float2 Uv : UV0;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
	float3 Binormal : BINORMAL;
	float Clip : SV_ClipDistance0;
};

GeometryInput VS(VertexInput input)
{
   
	GeometryInput output;
    //output.Color = input.Color;
	output.Uv = input.Uv;
	
	Matrix world;
    [flatten]
	if (input.Weights.x)
		world = SkinWorld(input.Indices, input.Weights);
	else
		world = World;
		
	output.Position = mul(input.Position, world);
	output.Clip = dot(output.Position, ClipPlane);
	
	
	output.wPosition = output.Position.xyz;
	//output.Position = mul(output.Position, ViewProj);
	//input.Normal.w = 0.0f;
	output.Normal = mul(input.Normal, (float3x3) world);
	//input.Tangent.w = 0.0f;
	output.Tangent = mul(input.Tangent, (float3x3) world);
	output.Binormal = cross(output.Normal.xyz, output.Tangent.xyz);
	
	return output;
}


[maxvertexcount(18)]
void GS(triangle GeometryInput input[3], inout TriangleStream<PixelInput> stream)
{
	int vertex = 0;
	PixelInput output;
    
    [unroll(6)]
	for (int i = 0; i < 6; i++)
	{
		output.TargetIndex = i;
        
        [unroll(3)]
		for (vertex = 0; vertex < 3; vertex++)
		{
			output.Position = input[vertex].Position;
			output.Position = mul(output.Position, (CubeViews[i]));
			output.Position = mul(output.Position, CubeProjection);
			output.Uv = input[vertex].Uv;
			output.Normal = input[vertex].Normal;
			output.Binormal = input[vertex].Binormal;
			output.Tangent = input[vertex].Tangent;
			output.wPosition = input[vertex].wPosition;
			output.Clip = input[vertex].Clip;
			stream.Append(output);
		}
        
		stream.RestartStrip();
	}
}


float4 PS(PixelInput input) : SV_TARGET
{
	float4 BaseColor = DiffuseMapping(input.Uv);
	float3 Normal = NormalMapping(input.Normal, input.Tangent, input.Binormal, input.Uv);
	BaseColor = Lighting(BaseColor, input.Uv, Normal, input.wPosition);
	return BaseColor;

}