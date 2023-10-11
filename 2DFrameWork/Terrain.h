#pragma once


class Terrain : public Actor
{
    struct InputDesc
    {
        UINT index;
        Vector3 v0, v1, v2;
    };
    struct OutputDesc
    {
        int picked;
        float u, v, distance;
    };
    struct RayDesc
    {
        Vector3 position;
        float size;
        Vector3 direction;
        float padding;
    };
    static ID3D11ComputeShader* computeShader;
    static class Actor* Node; //구 모양
    static class Actor* Line; //선 모양
public:
    static void		CreateStaticMember();
    static void		DeleteStaticMember();
    static Terrain* Create(string Name = "Terrain", int	terrainSize = 257, float uvScale = 1.0f);
private:
    //compute Input
    InputDesc* inputArray;
    ID3D11Resource* input;
    ID3D11ShaderResourceView* srv = nullptr;//읽기전용
    //compute Output
    OutputDesc* outputArray;
    ID3D11Resource* output;
    ID3D11UnorderedAccessView* uav;//읽기쓰기 둘다가능
    //copy용
    ID3D11Resource* result;
    //ray
    RayDesc                     ray;
    ID3D11Buffer*               rayBuffer;
public:
    Di                         di;
    Terrain(string Name);

    float                       uvScale;
    int                         triSize;
    bool                        showNode;
    bool                        depth;
    void	        RenderDetail();
    void            UpdateMeshUv();
    void            UpdateMeshNormal();

    void            CreateMesh(int Size = 257);
    //CS
    void            CreateStructuredBuffer();
    void            DeleteStructuredBuffer();

    bool            ComPutePicking(Ray WRay, OUT Vector3& HitPoint);
    void            Render() override;
    void            Render(shared_ptr<Shader> shader) override;


    bool            IntersectCollider(Ray WRay);
    int             GetDiNodeIdx(Vector3 pos);
    void            AutoNode(Vector3 localPos);

    bool PathFinding(int from, int to, std::list<Vector3>& way);
    void AllPathActivate() const;
    bool IntersectCol(Ray ray) const;
    int  FindNodeIdx(Vector3 pos) const;

    void AllLink(Vector3 pos);
};
