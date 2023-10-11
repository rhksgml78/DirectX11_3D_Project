#include "framework.h"

PostEffect::PostEffect()
{
    RTT = new RenderTarget();
    postEffect = Actor::Create();
    postEffect->LoadFile("PostEffect.xml");

    {
        D3D11_BUFFER_DESC desc = { 0 };
        desc.ByteWidth = sizeof(PSP);
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//�������
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;
        HRESULT hr = D3D->GetDevice()->CreateBuffer(&desc, NULL, &PEBuffer);
        assert(SUCCEEDED(hr));

    }

    psp.screen.x = App.GetHalfWidth();
    psp.screen.y = App.GetHalfHeight();
    psp.radius = 1200.0f;
    psp.color.x = 0.5f;
    psp.color.y = 0.5f;
    psp.color.z = 0.5f;
}

PostEffect::~PostEffect()
{
    delete RTT;
    postEffect->Release();
    PEBuffer->Release();
}

void PostEffect::Update()
{
    postEffect->Update();
}

void PostEffect::SetTarget()
{
    RTT->SetTarget(Color(0, 0, 0, 1));
}

void PostEffect::Render()
{
    RTT->SetRGBTexture(1);

    {
        //ector3 viewUp = GetUp();
        D3D11_MAPPED_SUBRESOURCE mappedResource;
        D3D->GetDC()->Map(PEBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
        memcpy_s(mappedResource.pData, sizeof(PSP), &psp, sizeof(PSP));
        D3D->GetDC()->Unmap(PEBuffer, 0);
        D3D->GetDC()->PSSetConstantBuffers(11, 1, &PEBuffer);
    }

    postEffect->Render();
}

void PostEffect::RenderDetail()
{
    ImGui::SliderInt("filter", &psp.filter, 0, 4);
    ImGui::ColorEdit3("color", (float*)&psp.color, ImGuiColorEditFlags_PickerHueWheel);
    ImGui::SliderFloat2("screen", (float*)&psp.screen,0,2000);
    ImGui::SliderFloat("radius", &psp.radius,0,2000);

    ImGui::SliderInt("select", &psp.select, 0, 7);
    ImGui::SliderInt("count", &psp.count,0,2000);
    ImGui::SliderFloat("width", &psp.width, 0, 2000);
    ImGui::SliderFloat("height", &psp.height, 0, 2000);
}
