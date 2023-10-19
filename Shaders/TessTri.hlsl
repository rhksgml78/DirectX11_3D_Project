#include "Common.hlsl"

struct VertexInput
{
    float4 Position : POSITION0;
};

struct CHullOutput
{
    float edgeTessFactor[3] : SV_TessFactor;
    float insideTessFactor : SV_InsideTessFactor;
};

struct HullOutput
{
    float4 Position : POSITION;
};

struct PixelInput
{
    float4 Position : SV_POSITION;
};


#define NUM_CONTROL_POINTS 3

cbuffer EdgeInfo : register(b10)
{
    float3 edges;
    float inside;
}



VertexInput VS(VertexInput input)
{
    // ���� �������� VS�ܰ迡 �ƹ����� ���°���
    // �ܼ��� NDC��ǥ�� �׸����׷� �׼����̼��� ���ظ��ϱ�����.
    // ������ ����Ϸ��� ������ ������ǥ���� ����Ǿ��־����!
    return input;
}

// CHS - Constant Hull Shader
CHullOutput CHS(InputPatch<VertexInput, NUM_CONTROL_POINTS> input)
{
    // ���ε� �Ǿ��ִ� �Լ� ����
    CHullOutput output;

    // ����� ������� ������ �״�� ���ԵǾ� �ִ�.
    // ���� ������ ����Ϸ��� ��� �ɰ������� ������̳�
    // ��� �����Ұ������� ���� �Ѵ�.
    output.edgeTessFactor[0] = edges.x;
    output.edgeTessFactor[1] = edges.y;
    output.edgeTessFactor[2] = edges.z;
    output.insideTessFactor = inside;

    return output;
}


[domain("tri")] // �ﰢ������ �׼����̼� - �簢���� quad
[partitioning("integer")] // ���Ҵ����� int (ũ�Թٲ�������)
[outputtopology("triangle_cw")] // Crock Wise �ð���� �ﰢ�� ��������
[outputcontrolpoints(3)] // �� ������ ���� �ɰ���
[patchconstantfunc("CHS")] // �潦�̴��� �Լ��� �� ���ε��ؼ�����ؾ��� ���Լ��� CHS

HullOutput HS(InputPatch<VertexInput, NUM_CONTROL_POINTS> input,
	uint i : SV_OutputControlPointID) // for�������� �����̴�. (��ǻ�ͽ��̴����� ����ϰ� ����)
{
    HullOutput output;

    output.Position = input[i].Position;

    return output;
}


[domain("tri")]
PixelInput DS(CHullOutput input, float3 uvw : SV_DomainLocation,
	const OutputPatch<HullOutput, NUM_CONTROL_POINTS> patch)
{
    PixelInput output;

    float4 pos = patch[0].Position * uvw.x + patch[1].Position * uvw.y + patch[2].Position * uvw.z;
    output.Position = float4(pos.xyz, 1.0f);

    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    return float4(1, 1, 0, 1);
}
