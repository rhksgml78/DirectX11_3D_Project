#include "Common.hlsl"

struct VertexInput
{
    float4 Position : POSITION0;
};
struct PixelInput
{
    float4 Position : SV_POSITION;
    float3 Uv : UV0;
};

PixelInput VS(VertexInput input)
{
   
    PixelInput output;
    output.Uv = normalize(input.Position.xyz);
    output.Position = input.Position;
    //  o           =  i X W
    output.Position.xyz = mul(output.Position.xyz, (float3x3) World);
	output.Position.xyz = mul(output.Position.xyz, (float3x3) View);
    output.Position = mul(output.Position, Proj);
    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    float4 BaseColor = float4(1, 1, 1, 1);
    
      //������ �ٶ󺸴� ����
	float3 Light = normalize(DirLight);
     //��ü���� ������ ����(��)
	float3 Nor = normalize(input.Uv.xyz);
    
     //������ ������ ������
	float d = saturate(dot(-Light, Nor));
	d = pow(d, 30.0f);
    
    
    //ť��ʿ��� ������ ��
	BaseColor = TextureBG.Sample(SamplerBG, input.Uv);
    
	BaseColor.rgb += float3(d, d, d);
    
    return BaseColor;
}