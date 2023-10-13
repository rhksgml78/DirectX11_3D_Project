#pragma once
class Deferred
{

private:
    RenderTarget* diffuseTarget;
    RenderTarget* specularTarget;
    RenderTarget* normalTarget;
    RenderTarget* posTarget;

    RenderTarget* rtvs[4];
    ID3D11ShaderResourceView* srvs[4];
public:
    Deferred(UINT width = App.GetWidth(),
        UINT height = App.GetHeight());
    ~Deferred();
    void SetTarget(Color clear = Color(0, 0, 0, 1));
    void SetRGBTexture(int slot);
    //창사이즈 조절
    void ResizeScreen(float width, float height);
    void RenderDetail();
};