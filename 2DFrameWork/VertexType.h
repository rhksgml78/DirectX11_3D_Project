#pragma once

enum class VertexType
{
    P,
    PC,
    PCN,
    PTN,
    MODEL,
    TERRAIN,
    PT,
    PS,
    PSV,
    PSP,
    //PCNI,
};

struct VertexP
{
    VertexP()
    {
    };
    VertexP(Vector3 pos)
    {
        position = pos;
    };
    Vector3 position;
    static D3D11_INPUT_ELEMENT_DESC LayoutDesc[];
};

struct VertexPC
{
    VertexPC()
    {
    };
    VertexPC(Vector3 pos, Color color)
    {
        this->position = pos;
        this->color = color;
    };
    Vector3 position;
    Color color;
    static D3D11_INPUT_ELEMENT_DESC LayoutDesc[];
};

struct VertexPCN
{
    VertexPCN()
    {
    };
    VertexPCN(Vector3 pos, Color color, Vector3 normal)
    {
        this->position = pos;
        this->color = color;
        this->normal = normal;
    };
    Vector3 position;
    Color color;
    Vector3 normal;
    static D3D11_INPUT_ELEMENT_DESC LayoutDesc[];
};
struct VertexPTN
{
    VertexPTN()
    {
    };
    VertexPTN(Vector3 pos, Vector2 uv, Vector3 normal)
    {
        this->position = pos;
        this->uv = uv;
        this->normal = normal;
    };
    Vector3 position;
    Vector2 uv;
    Vector3 normal;
    static D3D11_INPUT_ELEMENT_DESC LayoutDesc[];
};

struct VertexModel
{
    VertexModel()
    {
    };
    VertexModel(Vector3 pos, Vector2 uv, Vector3 normal,
        Vector3 tangent, Vector4 indices, Vector4 weights)
    {
        this->position = pos;
        this->uv = uv;
        this->normal = normal;
        this->tangent = tangent;
        this->indices = indices;
        this->weights = weights;
    };
    Vector3 position;
    Vector2 uv;
    Vector3 normal;
    Vector3 tangent;
    Vector4 indices;
    Vector4 weights;
    static D3D11_INPUT_ELEMENT_DESC LayoutDesc[];
};

struct VertexTerrain
{
    VertexTerrain()
    {
        weights.x = 1.0f;
        weights.y = 0.0f;
        weights.z = 0.0f;
        weights.w = 0.0f;
    };
    VertexTerrain(Vector3 pos, Vector2 uv, Vector3 normal, Vector4 weight)
    {
        this->position = pos;
        this->uv = uv;
        this->normal = normal;
        this->weights = weight;
    };
    Vector3 position;
    Vector2 uv;
    Vector3 normal;
    Vector4 weights; //두 텍스처를 얼마의비율로 섞을건지?
    //int     terrainId;
    static D3D11_INPUT_ELEMENT_DESC LayoutDesc[];
};

struct VertexPT
{
    VertexPT()
    {
    };
    VertexPT(Vector3 pos, Vector2 uv)
    {
        this->position = pos;
        this->uv = uv;
    };
    Vector3 position;
    Vector2 uv;
    static D3D11_INPUT_ELEMENT_DESC LayoutDesc[];
};

struct VertexPS
{
    VertexPS()
    {
    };
    VertexPS(Vector3 pos, Vector2 size)
    {
        this->position = pos;
        this->size = size;
    };
    Vector3 position;
    Vector2 size;
    static D3D11_INPUT_ELEMENT_DESC LayoutDesc[];
};

struct VertexPSV
{
    VertexPSV()
    {
    };
    VertexPSV(Vector3 pos, Vector2 size,Vector3 velocity)
    {
        this->position = pos;
        this->size = size;
        this->velocity = velocity;
    };
    Vector3 position;
    Vector2 size;
    Vector3 velocity;
    static D3D11_INPUT_ELEMENT_DESC LayoutDesc[];
};

struct VertexPSP
{
    VertexPSP()
    {
    };
    VertexPSP(Vector3 pos, Vector2 size, Vector2 pivot)
    {
        this->position = pos;
        this->size = size;
        this->pivot = pivot;
    };
    Vector3 position;
    Vector2 size;
    Vector2 pivot;
    static D3D11_INPUT_ELEMENT_DESC LayoutDesc[];
};

//struct VertexPCNI // 인스턴싱 타입
//{
//    static D3D11_INPUT_ELEMENT_DESC LayoutDesc[];
//};