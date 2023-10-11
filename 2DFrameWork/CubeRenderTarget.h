#pragma once
class CubeRenderTarget
{

protected:
    int width, height;

    ID3D11Texture2D* rgb;
    ID3D11Texture2D* depth;

    ID3D11RenderTargetView* rgbTarget;
    ID3D11DepthStencilView* depthTarget;

    ID3D11ShaderResourceView* rgbResource;

    ID3D11SamplerState* sampler;

    //창사이즈 조절
    void CreateBackBuffer(float width, float height);
    void DeleteBackBuffer();

public:
    CubeRenderTarget(UINT width = 256,
        UINT height = 256);
    ~CubeRenderTarget();

    virtual void SetTarget(Color clear = Color(0, 0, 0, 1));
    virtual void SetRGBTexture(int slot);


    virtual void ResizeScreen(float width, float height);
};

