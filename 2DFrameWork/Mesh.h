#pragma once
class Mesh
{
    friend class GameObject;
    friend class Collider;
public:
    ID3D11Buffer*           vertexBuffer;
    ID3D11Buffer*           indexBuffer;
    ID3D11Buffer*           instanceBuffer; // 인스턴싱용 버퍼 추가

    UINT*                   indices;
    void*                   vertices;
    Matrix*                 instance; // 인스턴스 매트릭스

    UINT                    vertexCount;
    UINT                    indexCount;
    UINT                    instanceCount; // 인스턴스 갯수

    D3D_PRIMITIVE_TOPOLOGY  primitiveTopology;
    VertexType              vertexType;
    UINT                    byteWidth;

    string                  file;
    string                  instanceFile;

public:
    Mesh();
    Mesh(void* vertices, UINT vertexCount, UINT* indices,
        UINT indexCount, VertexType type);
    ~Mesh();
    void Set();
    void LoadFile(string file);
    void SaveFile(string file);
    //인스턴스 파일 로드 및 저장
    void LoadInstanceFile(string file);
    void SaveInstanceFile(string file);
    void Reset();
    const Vector3& GetVertexPosition(UINT idx);
    Vector3& SetVertexPosition(UINT idx);
    void Update();

    // 인스턴싱용 버퍼생성과 드로우콜 추가
    void CreateInstanceBuffer(Matrix* instance, UINT instanceCount);
    void DrawCall();

    //09.22 추가
    void RenderDetail();
    void InstanceEdit();
};

