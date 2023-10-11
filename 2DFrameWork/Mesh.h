#pragma once
class Mesh
{
    friend class GameObject;
    friend class Collider;
public:
    ID3D11Buffer*           vertexBuffer;
    ID3D11Buffer*           indexBuffer;
    ID3D11Buffer*           instanceBuffer; // �ν��Ͻ̿� ���� �߰�

    UINT*                   indices;
    void*                   vertices;
    Matrix*                 instance; // �ν��Ͻ� ��Ʈ����

    UINT                    vertexCount;
    UINT                    indexCount;
    UINT                    instanceCount; // �ν��Ͻ� ����

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
    //�ν��Ͻ� ���� �ε� �� ����
    void LoadInstanceFile(string file);
    void SaveInstanceFile(string file);
    void Reset();
    const Vector3& GetVertexPosition(UINT idx);
    Vector3& SetVertexPosition(UINT idx);
    void Update();

    // �ν��Ͻ̿� ���ۻ����� ��ο��� �߰�
    void CreateInstanceBuffer(Matrix* instance, UINT instanceCount);
    void DrawCall();

    //09.22 �߰�
    void RenderDetail();
    void InstanceEdit();
};

