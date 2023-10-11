#include "Framework.h"

Texture::Texture()
{
    Sampler = nullptr;
    srv = nullptr;
    //기본 샘플러 값
    //SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    SamplerDesc.MipLODBias = 0.0f;
    SamplerDesc.MaxAnisotropy = 1;
    SamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    SamplerDesc.MinLOD = -FLT_MAX;
    SamplerDesc.MaxLOD = FLT_MAX;
    CreateSampler();
}

Texture::~Texture()
{
    SafeRelease(srv);
    SafeRelease(Sampler);
}

void Texture::RenderDetail()
{
    ImGui::Text(file.c_str());
    ImVec2 size(400, 400);
    ImGui::Image((void*)srv, size);

    ImGui::PushID(this);
    if (ImGui::Button("SamplerButton"))
    {
        ImGui::OpenPopup("Sampler");
    }
    if (ImGui::BeginPopup("Sampler"))
    {
        if (ImGui::Button("Filter POINT"))
        {
            SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
            CreateSampler();
        }
        if (ImGui::Button("Filter LINEAR"))
        {
            SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
            CreateSampler();
        }
        if (ImGui::Button("AddressU Clamp"))
        {
            SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
            CreateSampler();
        }
        if (ImGui::Button("AddressU Wrap"))
        {
            SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
            CreateSampler();
        }
        if (ImGui::Button("AddressU Mirror"))
        {
            SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
            CreateSampler();
        }
        if (ImGui::Button("AddressV Clamp"))
        {
            SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
            CreateSampler();
        }
        if (ImGui::Button("AddressV Wrap"))
        {
            SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
            CreateSampler();
        }
        if (ImGui::Button("AddressV Mirror"))
        {
            SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
            CreateSampler();
        }
        ImGui::EndPopup();
    }
    ImGui::PopID();
}

void Texture::LoadFile(string file)
{
    this->file = file;
    size_t index = file.find_last_of('.');
    //확장자 문자열 자르기
    string format = file.substr(index + 1, file.length());
    wstring path = L"../Contents/Texture/" + Util::ToWString(file);

    ScratchImage image;
    HRESULT hr;
    if (format == "tga")
        hr = LoadFromTGAFile(path.c_str(), nullptr, image);
    else if (format == "dds")
        hr = LoadFromDDSFile(path.c_str(), DDS_FLAGS_NONE, nullptr, image);
    else
        hr = LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, nullptr, image);
    Check(hr);

    CreateShaderResourceView(D3D->GetDevice(), image.GetImages(), image.GetImageCount(),
        image.GetMetadata(), &srv);
}

void Texture::CreateSampler()
{
    SafeRelease(Sampler);
    HRESULT hr;

    hr = D3D->GetDevice()->CreateSamplerState(&SamplerDesc, &Sampler);
    Check(hr);
}

void Texture::Set(int slot)
{
    D3D->GetDC()->PSSetShaderResources(slot,// 연결될레지스터 번호 0
        1,//리소스갯수
        &srv);
    D3D->GetDC()->PSSetSamplers(slot, 1, &Sampler);
}
