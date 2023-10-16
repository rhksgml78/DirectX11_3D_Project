#include "Common.hlsl"

cbuffer PS_Water : register(b10)
{
	float2 dir;
	float time; //흐르는 시간 속도
	float scale; // uv크기
    float refractscalar; // 굴절률
    float3 padding;
}


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

float4 PS(PixelInput input) : SV_TARGET
{
    float2 Uv = (input.Uv + dir * time)*scale;
	
	
	float4 BaseColor = DiffuseMapping(Uv);
	
	
	float3 MappingNormal = NormalMapping(input.Normal, input.Tangent, input.Binormal, Uv);
	float3 Normal = normalize(input.Normal);
	
	float3 SpecularMap = SpecularMapping(Uv);
	float4 Result = float4(DirLighting(BaseColor.rgb, SpecularMap,
    MappingNormal, input.wPosition),
    BaseColor.a);
	
	if (Result.a > Opacity)
		Result.a = Opacity;
	
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
			Result.rgb += PointLighting(BaseColor.rgb, SpecularMap,
			MappingNormal, input.wPosition, i);
		}
		else if (lights[i].Type == 1)
		{
			Result.rgb += SpotLighting(BaseColor.rgb, SpecularMap,
            MappingNormal, input.wPosition, i);
		}
	}
	
	//발광
	//Ambient
	Result.rgb += Ka.rgb * BaseColor.rgb;
    //Emissive
	Result.rgb += EmissiveMapping(BaseColor.rgb, Uv, MappingNormal, input.wPosition);
	//Environment
	
    float3 Envi = float3(0, 0, 0);
    float3 Envi2 = float3(0, 0, 0);
	
	[flatten]
	if (environment != 0.0f)
	{
		// 시야벡터
		float3 ViewDir = normalize(input.wPosition - ViewPos.xyz);
		// 반사
		float3 reflection = reflect(ViewDir, Normal);
		// 법선
		float3 normal = TextureN.Sample(SamplerN, Uv).rgb;
		
		normal = normal * 2.0f - 1.0f; // 0.0 ~ 1.0 구간으로 제한
		
        reflection.xyz += MappingNormal * 0.03f;
        reflection = normalize(reflection);
		
        //float3 Refract = refract(normalize(ViewDir), Normal, 1) + MappingNormal * 0.03f;
		
        float3 Refract = refract(normalize(ViewDir), Normal, 1) + MappingNormal * refractscalar;
		
        Envi = TextureBG.Sample(SamplerBG, reflection.xyz);
        Envi2 = TextureC.Sample(SamplerC, Refract);
    }
	/*
	refract() 함수를 사용한다! 매개변수3개들어감 float3형태로 i n n
	입사각 표면 굴절률 (물체에따른 굴절률은 인터넷자료참고. 물은1.333)
	쉐이더코드에서 굴절률을적용할떄에는 1.01정도가 적당한것같다. 
	*/
    if (padding.r == 1)
    {
        Result.rgb = Result.rgb * (1.0f - environment) + (Envi) * environment;
    }
    if (padding.g == 1)
    {
        Result.rgb = Result.rgb * (1.0f - environment) + (Envi2) * environment;
    }
    if (padding.b == 1)
    {
        Result.rgb = Result.rgb * (1.0f - environment) + (Envi + Envi2) * environment * 0.5f;
    }
	
	// RTT1 + 2 = 완성본
    //Result.rgb = Result.rgb * (1.0f - environment) + (Envi + Envi2) * environment * 0.5f;
	
	BaseColor = Result;
	
	
	return BaseColor;

}