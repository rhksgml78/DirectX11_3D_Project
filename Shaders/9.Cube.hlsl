#include "Common.hlsl"

struct VertexInput
{
    float4 Position : POSITION0;
    float2 Size : SIZE0;
    float2 Pivot : PIVOT0;
};
struct PixelInput
{
    float4 Position : SV_POSITION;
    float2 Uv : UV0;
};

VertexInput VS(VertexInput input)
{
   
    VertexInput output;
    output.Size = input.Size;
    output.Pivot = input.Pivot;
    //  o           =  i X W
    output.Position = mul(input.Position, World);
	output.Position = mul(output.Position, View);
    return output;
}

static const float2 TEXCOORD[4] =
{
    float2(0.0f, 1.0f),
    float2(0.0f, 0.0f),
    float2(1.0f, 1.0f),
    float2(1.0f, 0.0f)
};
[maxvertexcount(4)]
void GS(point VertexInput input[1], inout TriangleStream<PixelInput> output)
{
    //�Ѱ��� ���� �װ��� ������
    
    // ���庯ȯ�� �� �������Ǻ�ȯ
    
    //float3 up = GSViewUp;
    //float3 forward = GSViewForward;
    //float3 right = normalize(cross(up, forward));
    
	float3 up = float3(0, 1, 0);
	float3 forward = float3(0, 0, 1);
	float3 right = float3(1, 0, 0);
    
	float2 halfSize = input[0].Size * 0.5f;
    
    float4 vertices[4]; //(������ǥ,����)
    
	float3 pivot = input[0].Position.xyz;
	pivot.x -= input[0].Pivot.x * halfSize.x;
	pivot.y -= input[0].Pivot.y * halfSize.y;
    
    //���� �Ʒ�
	vertices[0] = float4(pivot - right * halfSize.x - up * halfSize.y, 1.0f);
    // �� ��
	vertices[1] = float4(pivot - right * halfSize.x + up * halfSize.y, 1.0f);
    // �� �Ʒ�
	vertices[2] = float4(pivot + right * halfSize.x - up * halfSize.y, 1.0f);
    // �� ��
	vertices[3] = float4(pivot + right * halfSize.x + up * halfSize.y, 1.0f);
    
    PixelInput pixelInput;
    
    [unroll(4)]
    for (int i = 0; i < 4; i++)
    {
        //���忡�� �ٽ� ndc���� ��ȯ
        pixelInput.Position = mul(vertices[i], GSProj);
        pixelInput.Uv = TEXCOORD[i];
        
        output.Append(pixelInput);
    }
}

float4 PS(PixelInput input) : SV_TARGET
{
    
    float4 BaseColor = DiffuseMapping(input.Uv);
    
    if (BaseColor.a == 0)
        discard;
    return BaseColor;
}