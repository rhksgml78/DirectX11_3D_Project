#pragma once
class Environment : public CubeRenderTarget
{
private:
    static ID3D11Buffer* camBuffer;
    //static ID3D11Buffer* PSBuffer;

    struct GSCamBuffer
    {
        Matrix Views[6];
        Matrix Projection;
    } desc;
    Viewport    viewport;

public:
    Vector3     position;
    float		fov, nearZ, farZ;

    Environment(UINT width = 256, UINT height = 256);
    ~Environment();
    shared_ptr<Texture> environment;
    static void CreateStaticMember();
    static void DeleteStaticMember();
    virtual void ResizeScreen(float width, float height);
    void RenderDetail();

    void SetTarget(Color clear = Color(0, 0, 0, 1));
    void SetRGBTexture(int slot);
};

