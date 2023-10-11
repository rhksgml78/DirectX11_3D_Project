#pragma once
class SlashTrail
{
    shared_ptr<Shader>      shader;

    vector<VertexPT>        vertices;
    ID3D11Buffer* vertexBuffer;
    D3D_PRIMITIVE_TOPOLOGY  primitiveTopology;
    VertexType              vertexType;
    UINT                    byteWidth;

    Vector3 lastTop;
    Vector3 lastBottom;

    float       time;
public:
    shared_ptr<Material>      material;
    float       interval;       //���� ����
    int         maxTrail;       //�簢�� ����
    bool        isPlaying;
    GameObject* Top = nullptr;
    GameObject* Bottom = nullptr;
    SlashTrail();
    ~SlashTrail();
    void Play();
    void Stop();
    void Update(); //�ܻ�޽� ����
    void Render(); //�ܻ�޽� ����
    void RenderDetail();
};

