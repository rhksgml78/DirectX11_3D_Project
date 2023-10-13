#include "framework.h"

Deferred::Deferred(UINT width, UINT height)
{
    diffuseTarget = new RenderTarget(width, height);
    specularTarget = new RenderTarget(width, height);
    normalTarget = new RenderTarget(width, height);
    //depthStencil = new RenderTarget(width, height);
    posTarget = new RenderTarget(width, height);

    rtvs[0] = diffuseTarget;
    rtvs[1] = specularTarget;
    rtvs[2] = normalTarget;
    rtvs[3] = posTarget;

    srvs[0] = diffuseTarget->rgbResource;
    srvs[1] = specularTarget->rgbResource;
    srvs[2] = normalTarget->rgbResource;
    srvs[3] = posTarget->rgbResource;

}

Deferred::~Deferred()
{
    delete diffuseTarget;
    delete specularTarget;
    delete normalTarget;
    delete posTarget;
}

void Deferred::SetTarget(Color clear)
{
    vector<ID3D11RenderTargetView*> vecRtvs;

    for (int i = 0; i < 4; i++)
    {
        vecRtvs.push_back(rtvs[i]->rgbTarget);
        D3D->GetDC()->ClearRenderTargetView(vecRtvs.back(), (float*)&clear);
    }
    D3D->GetDC()->ClearDepthStencilView(posTarget->depthTarget, D3D11_CLEAR_DEPTH, 1.0f, 0);

    D3D->GetDC()->OMSetRenderTargets(4, vecRtvs.data(), posTarget->depthTarget);
}

void Deferred::SetRGBTexture(int slot)
{
    for (int i = 0; i < 4; i++)
    {
        D3D->GetDC()->PSSetShaderResources(slot + i, 1, &srvs[i]);
    }
}

void Deferred::ResizeScreen(float width, float height)
{
    diffuseTarget->ResizeScreen(width, height);
    specularTarget->ResizeScreen(width, height);
    normalTarget->ResizeScreen(width, height);
    posTarget->ResizeScreen(width, height);
}

void Deferred::RenderDetail()
{
    ImVec2 size(400, 400);
    ImGui::Image((void*)diffuseTarget->rgbResource, size);
    ImGui::Image((void*)specularTarget->rgbResource, size);
    ImGui::Image((void*)normalTarget->rgbResource, size);
    ImGui::Image((void*)posTarget->depthResource, size);
}
