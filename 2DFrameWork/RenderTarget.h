#pragma once
class RenderTarget
{
protected:
    int width, height;

    ID3D11Texture2D* rgb;
    ID3D11Texture2D* depth;

    //���������� �����Ҷ� Ÿ������ �� �������̽�
    ID3D11RenderTargetView* rgbTarget;
    ID3D11DepthStencilView* depthTarget;

    //���������� �����Ҷ� �ؽ��ķ� �� �������̽�
    ID3D11ShaderResourceView* rgbResource;
    ID3D11ShaderResourceView* depthResource;

    ID3D11SamplerState* sampler;
    
    //â������ ����
    void CreateBackBuffer(float width, float height);
    void DeleteBackBuffer();

public:
    RenderTarget(UINT width = App.GetWidth(),
        UINT height = App.GetHeight());
    ~RenderTarget();


    void SetTarget(Color clear = Color(0, 0, 0, 1));
    void SetRGBTexture(int slot);


    //â������ ����
    void ResizeScreen(float width, float height);
};

