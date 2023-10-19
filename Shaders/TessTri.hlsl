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
    // 현재 예제에서 VS단계에 아무값도 없는것은
    // 단순히 NDC좌표에 그림을그려 테셀레이션을 이해만하기위함.
    // 실제로 사용하려면 정점이 월드좌표까지 변경되어있어야함!
    return input;
}

// CHS - Constant Hull Shader
CHullOutput CHS(InputPatch<VertexInput, NUM_CONTROL_POINTS> input)
{
    // 바인딩 되어있는 함수 내부
    CHullOutput output;

    // 현재는 상수버퍼 값으로 그대로 대입되어 있다.
    // 만일 실제로 사용하려면 어떻게 쪼갤것인지 계수값이나
    // 어떻게 보간할것인지가 들어가야 한다.
    output.edgeTessFactor[0] = edges.x;
    output.edgeTessFactor[1] = edges.y;
    output.edgeTessFactor[2] = edges.z;
    output.insideTessFactor = inside;

    return output;
}


[domain("tri")] // 삼각형단위 테셀레이션 - 사각형은 quad
[partitioning("integer")] // 분할단위는 int (크게바뀌지않음)
[outputtopology("triangle_cw")] // Crock Wise 시계방향 삼각형 토폴로지
[outputcontrolpoints(3)] // 점 세개로 나눠 쪼갤것
[patchconstantfunc("CHS")] // 헐쉐이더는 함수를 꼭 바인딩해서사용해야함 그함수가 CHS

HullOutput HS(InputPatch<VertexInput, NUM_CONTROL_POINTS> input,
	uint i : SV_OutputControlPointID) // for문과같은 성능이다. (컴퓨터쉐이더에서 비슷하게 썼음)
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
