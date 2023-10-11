#include "framework.h"

Environment::Environment(UINT width, UINT height)
	: CubeRenderTarget(width, height)
{
	environment = make_shared<Texture>();
	viewport.width = (float)width;
	viewport.height = (float)height;

	fov = 90.0f * TORADIAN;
	nearZ = 0.001f;
	farZ = 10000.0f;

}

Environment::~Environment()
{
	SafeReset(environment);
}
ID3D11Buffer* Environment::camBuffer = nullptr;
void Environment::CreateStaticMember()
{
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.ByteWidth = sizeof(GSCamBuffer);
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//상수버퍼
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		HRESULT hr = D3D->GetDevice()->CreateBuffer(&desc, NULL, &camBuffer);
		assert(SUCCEEDED(hr));
	}
}

void Environment::DeleteStaticMember()
{
	SafeRelease(camBuffer);
}

void Environment::ResizeScreen(float width, float height)
{
	viewport.width = (float)width;
	viewport.height = (float)height;
	CubeRenderTarget::ResizeScreen(width, height);
}

void Environment::RenderDetail()
{
	if (ImGui::SliderFloat3("position", (float*)&position, -1000, 1000))
	{
	}

	if (ImGui::SliderInt("TextureWidth", &width, 1, 2000))
	{
		ResizeScreen(width, height);
	}
	if (ImGui::SliderInt("TextureHeight", &height, 1, 2000))
	{
		ResizeScreen(width, height);
	}
}

void Environment::SetTarget(Color clear)
{
	//Create Views
	{
		float x = position.x;
		float y = position.y;
		float z = position.z;

		struct LookAt
		{
			Vector3 LookAt;
			Vector3 Up;
		} lookAt[6];

		lookAt[0] = { Vector3(x + 1.0f, y, z), Vector3(0, 1.0f, 0) };
		lookAt[1] = { Vector3(x - 1.0f, y, z), Vector3(0, 1.0f, 0) };
		lookAt[2] = { Vector3(x, y + 1.0f, z), Vector3(0, 0, -1.0f) };
		lookAt[3] = { Vector3(x, y - 1.0f, z), Vector3(0, 0, +1.0f) };
		lookAt[4] = { Vector3(x, y, z + 1.0f), Vector3(0, 1.0f, 0) };
		lookAt[5] = { Vector3(x, y, z - 1.0f), Vector3(0, 1.0f, 0) };

		for (UINT i = 0; i < 6; i++)
		{
			desc.Views[i] =
				Matrix::CreateLookAt(position, lookAt[i].LookAt, lookAt[i].Up);
			desc.Views[i] = desc.Views[i].Transpose();
		}
	}
	desc.Projection =
		Matrix::CreatePerspectiveFieldOfView(fov, width / height, nearZ, farZ);

	desc.Projection = desc.Projection.Transpose();

	D3D->GetDC()->RSSetViewports(1, viewport.Get11());

	/*desc2.Rotation = Matrix::CreateFromYawPitchRoll(rot.y, rot.x, rot.z);
	desc2.Rotation = desc2.Rotation.Transpose();*/

	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		D3D->GetDC()->Map(camBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		memcpy_s(mappedResource.pData, sizeof(GSCamBuffer), &desc, sizeof(GSCamBuffer));
		D3D->GetDC()->Unmap(camBuffer, 0);
		D3D->GetDC()->GSSetConstantBuffers(1, 1, &camBuffer);
	}

	CubeRenderTarget::SetTarget(clear);
}

void Environment::SetRGBTexture(int slot)
{

	CubeRenderTarget::SetRGBTexture(slot);
}
