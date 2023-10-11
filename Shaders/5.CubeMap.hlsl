#include "Common.hlsl"


struct VertexInput
{
    float4 Position : POSITION0;
    float2 Uv : UV0;
	float3 Normal : NORMAL0;
	float4 Weights : WEIGHTS0;
};

struct GeometryInput
{
	float4 Position : SV_POSITION;
	float3 Normal : NORMAL0;
	float3 wPosition : POSITION0;
	float2 Uv : UV0;
	float4 Weights : WEIGHTS0;
};

struct PixelInput
{
	uint TargetIndex : SV_RenderTargetArrayIndex;
    float4 Position : SV_POSITION;
	float3 Normal : NORMAL0;
	float3 wPosition : POSITION0;
    float2 Uv : UV0;
	float4 Weights : WEIGHTS0;
};

GeometryInput VS(VertexInput input)
{
   
	GeometryInput output;
    //output.Color = input.Color;
    output.Uv = input.Uv;
	//output.Color.a = 1.0f;
    //  o           =  i X W
    output.Position = mul(input.Position, World);
	output.wPosition = output.Position.xyz;
    //output.Position = mul(output.Position, ViewProj);
	output.Normal = mul(input.Normal, (float3x3) World);
	output.Weights = input.Weights;
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
			output.Weights = input[vertex].Weights;
			output.wPosition = input[vertex].wPosition;
			stream.Append(output);
		}
        
		stream.RestartStrip();
	}
}





float4 PS(PixelInput input) : SV_TARGET
{
	float4 BaseColor = 0;
    
     [flatten]
	if (Kd.a)
		BaseColor += TextureD.Sample(SamplerD, input.Uv) * input.Weights.y;
      [flatten]
	if (Ks.a)
		BaseColor += TextureS.Sample(SamplerS, input.Uv) * input.Weights.z;
      [flatten]
	if (Ka.a)
		BaseColor += TextureN.Sample(SamplerN, input.Uv) * input.Weights.x;
      [flatten]
	if (Ke.a)
		BaseColor += TextureE.Sample(SamplerE, input.Uv) * input.Weights.w;
   
	float3 SpecularMap = float3(1, 1, 1);
	
	float4 Result = float4(DirLighting(BaseColor.rgb, SpecularMap,
    normalize(input.Normal.xyz), input.wPosition),
    BaseColor.a);
	
	
	
	if (Result.a > Opacity)
		Result.a = Opacity;
	
	//±¤¿ø ¼ö¸¸Å­ ¹Ýº¹
	for (int i = 0; i < lights[0].Size; i++)
	{
        [flatten]
		if (!lights[i].isActive)
			continue;
        
        [flatten]
		if (lights[i].Type == 0)
		{
			Result.rgb += PointLighting(BaseColor.rgb, SpecularMap,
            input.Normal.xyz, input.wPosition, i);
		}
		else if (lights[i].Type == 1)
		{
			Result.rgb += SpotLighting(BaseColor.rgb, SpecularMap,
            input.Normal.xyz, input.wPosition, i);
		}
	}
	
	//¹ß±¤
	//Ambient
	Result.rgb += Ka.rgb * BaseColor.rgb;
    //Emissive
	//Result.rgb += EmissiveMapping(BaseColor.rgb, Uv, Normal, wPosition);
	
	return Result;
}