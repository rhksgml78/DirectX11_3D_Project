#include "framework.h"

void CubeRenderTarget::CreateBackBuffer(float width, float height)
{
	DXGI_FORMAT rtvFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	//Create rgb
	{
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
		desc.Width = width;
		desc.Height = height;
		desc.ArraySize = 6;
		desc.Format = rtvFormat;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
		desc.MipLevels = 1;
		desc.SampleDesc.Count = 1;

		Check(D3D->GetDevice()->CreateTexture2D(&desc, NULL, &rgb));
	}

	//Create rgb Target
	{
		D3D11_RENDER_TARGET_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
		desc.Format = rtvFormat;
		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
		desc.Texture2DArray.ArraySize = 6;

		Check(D3D->GetDevice()->CreateRenderTargetView(rgb, &desc, &rgbTarget));
	}
	//Create rgb SRV
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		desc.Format = rtvFormat;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		desc.TextureCube.MipLevels = 1;

		Check(D3D->GetDevice()->CreateShaderResourceView(rgb, &desc, &rgbResource));
	}
	DXGI_FORMAT dsvFormat = DXGI_FORMAT_D32_FLOAT;
	//Create depth Texture

	{
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
		desc.Width = width;
		desc.Height = height;
		desc.ArraySize = 6;
		desc.Format = dsvFormat;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
		desc.MipLevels = 1;
		desc.SampleDesc.Count = 1;

		Check(D3D->GetDevice()->CreateTexture2D(&desc, NULL, &depth));
	}


	//CreateDSV
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		desc.Format = dsvFormat;
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
		desc.Texture2DArray.ArraySize = 6;

		Check(D3D->GetDevice()->CreateDepthStencilView(depth, &desc, &depthTarget));
	}

	this->width = (float)width;
	this->height = (float)height;
}

void CubeRenderTarget::DeleteBackBuffer()
{
	SafeRelease(rgbTarget);
	SafeRelease(depthTarget);
	SafeRelease(rgbResource);
	//SafeRelease(depthResource);
	SafeRelease(rgb);
	SafeRelease(depth);
}

CubeRenderTarget::CubeRenderTarget(UINT width, UINT height)
{
	CreateBackBuffer(width, height);

	{
		D3D11_SAMPLER_DESC SamplerDesc;

		SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		SamplerDesc.MipLODBias = 0.0f;
		SamplerDesc.MaxAnisotropy = 1;
		SamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		SamplerDesc.MinLOD = -FLT_MAX;
		SamplerDesc.MaxLOD = FLT_MAX;
		D3D->GetDevice()->CreateSamplerState(&SamplerDesc, &sampler);
	}
}

CubeRenderTarget::~CubeRenderTarget()
{
	DeleteBackBuffer();
}

void CubeRenderTarget::SetTarget(Color clear)
{
	D3D->SetRenderTarget(rgbTarget, depthTarget);
	D3D->Clear(clear, rgbTarget, depthTarget);
}

void CubeRenderTarget::SetRGBTexture(int slot)
{
	D3D->GetDC()->PSSetShaderResources(slot, 1, &rgbResource);
	D3D->GetDC()->PSSetSamplers(slot, 1, &sampler);
}

void CubeRenderTarget::ResizeScreen(float width, float height)
{
	if (width < 1 || height < 1)
		return;

	this->width = width;
	this->height = height;

	DeleteBackBuffer();
	CreateBackBuffer(width, height);
}
