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
    return input;
}

CHullOutput CHS(InputPatch<VertexInput, NUM_CONTROL_POINTS> input)
{
    CHullOutput output;

    output.edgeTessFactor[0] = edges.x;
    output.edgeTessFactor[1] = edges.y;
    output.edgeTessFactor[2] = edges.z;
    output.insideTessFactor = inside;

    return output;
}


[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("CHS")]
HullOutput HS(InputPatch<VertexInput, NUM_CONTROL_POINTS> input,
	uint i : SV_OutputControlPointID)
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
