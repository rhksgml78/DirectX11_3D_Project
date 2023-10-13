#include "Common.hlsl"

struct VertexInput
{
	float4 Position : POSITION0;
	float2 Uv : UV0;
};
struct PixelInput
{
	float4 Position : SV_POSITION;
	float2 Uv : UV0;
};

PixelInput VS(VertexInput input)
{
   
	PixelInput output;
    output.Uv = input.Uv;
    //  o           =  i X W
    output.Position = mul(input.Position, World);
    return output;
}


Texture2D diffuseTexture : register(t10);
Texture2D specularTexture : register(t11);
Texture2D normalTexture : register(t12);
Texture2D posTexture : register(t13);

float4 PS(PixelInput input) : SV_TARGET
{
	float4 BaseColor = diffuseTexture.Sample(SamplerDefault, input.Uv);
	float3 Normal = normalTexture.Sample(SamplerDefault, input.Uv).xyz;
	float3 Specular = specularTexture.Sample(SamplerDefault, input.Uv).xyz;
	float3 wPosition = posTexture.Sample(SamplerDefault, input.Uv).xyz;
	
	//BaseColor = Lighting(BaseColor, input.Uv, Normal, wPos);
	
	
	float4 Result = float4(DirLighting(BaseColor.rgb, Specular,
    Normal, wPosition),
    BaseColor.a);
	
	//광원 수만큼
	//반복

	for (int i = 0; i < lights[0].Size; i++)
	{
        [flatten]
		if (!lights[i].isActive)
			continue;
        
        [flatten]
		if (lights[i].Type == 0)
		{
			Result.rgb += PointLighting(BaseColor.rgb, Specular,
			Normal, wPosition, i);
		}
		else if (lights[i].Type == 1)
		{
			Result.rgb += SpotLighting(BaseColor.rgb, Specular,
            Normal, wPosition, i);
		}
	}
    
	return Result;
}