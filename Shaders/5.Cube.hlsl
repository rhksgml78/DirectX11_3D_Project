#include "Common.hlsl"


struct VertexInput
{
    float4 Position : POSITION0;
    float2 Uv : UV;
	float3 Normal : NORMAL0;
	float4 Weights : WEIGHTS;
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



cbuffer PS_Brush : register(b10)
{
	float3 BrushPoint;
	float BrushRange;
	
	float BrushShape;
	float BrushType;
	float2 BrushPadding;
}


PixelInput VS(VertexInput input)
{
   
    PixelInput output;
    //output.Color = input.Color;
    output.Uv = input.Uv;
	//output.Color.a = 1.0f;
    //  o           =  i X W
    output.Position = mul(input.Position, World);
    output.vPosition = mul(output.Position, ShadowVP);
	output.wPosition = output.Position.xyz;
    output.Position = mul(output.Position, ViewProj);
	output.Normal = mul(input.Normal, (float3x3) World);
	output.Weights = input.Weights;
	
	//output.vPosition.xyz /= output.vPosition.w;
	//output.vPosition.x * viewport.width;
	//output.vPosition.y * viewport.height;
	
    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
	float4 BaseColor = 0;
    
	//return float4(1, 0, 0, 1);
	
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
    
	
	//float3 v1 = float3(BrushPoint.x, 0.0f, BrushPoint.z);
	//float3 v2 = float3(input.wPosition.x, 0.0f, input.wPosition.z);
	//float Dis = distance(v2, v1);
	//float w;
	//if (BrushShape == 1.0f)
	//{
	//	if (abs(v1.x - v2.x) < BrushRange
 //           && abs(v1.z - v2.z) < BrushRange)
	//	{
			
	//		if (BrushType == 0)
	//		{
	//			BaseColor.rgb += float3(1, 0, 0);
	//		}
	//		if (BrushType == 1)
	//		{
	//			BaseColor.rgb += float3(0, 1, 0);
	//		}
	//		if (BrushType == 2)
	//		{
	//			BaseColor.rgb += float3(0, 0, 1);
	//		}
	//	}
	//}
	//else
	//{
	//	w = saturate(Dis / BrushRange);
	//	w = 1.0f - w;
	//	if (BrushType == 0)
	//	{
	//		BaseColor.rgb += float3(w, 0, 0);
	//	}
	//	if (BrushType == 1)
	//	{
	//		BaseColor.rgb += float3(0, w, 0);
	//	}
	//	if (BrushType == 2)
	//	{
	//		BaseColor.rgb += float3(0, 0, w);
	//	}
	//}
	
	
    
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
	
    Result.rgb = AddShadow(Result, input.vPosition);

    //Emissive
	//Result.rgb += EmissiveMapping(BaseColor.rgb, Uv, Normal, wPosition);
	
	return Result;
}