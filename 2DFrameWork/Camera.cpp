#include "framework.h"

Camera::Camera()
{
	fov = 60.0f * TORADIAN;
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	width= App.GetWidth();
	height = App.GetHeight();
	viewport.width = App.GetWidth();
	viewport.height = App.GetHeight();
	nearZ = 0.1f;
	farZ = 500.0f;
	type = ObType::Camera;
	ortho = false;
}
Camera::~Camera()
{
}

Camera* Camera::Create(string name)
{
	Camera* temp = new Camera();
	temp->name = name;
	return temp;
}

void Camera::Update()
{
    GameObject::Update();
}
void Camera::Set()
{
	{
		view = (S.Invert()*W).Invert();
		if (ortho)
			proj = Matrix::CreateOrthographic(width, height, nearZ, farZ);
		else
			proj = Matrix::CreatePerspectiveFieldOfView(fov, width / height, nearZ, farZ);
		Matrix TV = view.Transpose();
		Matrix TP = proj.Transpose();
		Matrix TVP = view * proj;
		TVP = TVP.Transpose();
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		D3D->GetDC()->Map(VPBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		memcpy_s(mappedResource.pData, sizeof(Matrix), &TVP, sizeof(Matrix));
		D3D->GetDC()->Unmap(VPBuffer, 0);

		D3D11_MAPPED_SUBRESOURCE mappedResource2;
		D3D->GetDC()->Map(PBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource2);
		memcpy_s(mappedResource2.pData, sizeof(Matrix), &TP, sizeof(Matrix));
		D3D->GetDC()->Unmap(PBuffer, 0);

		D3D11_MAPPED_SUBRESOURCE mappedResource3;
		D3D->GetDC()->Map(VBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource3);
		memcpy_s(mappedResource3.pData, sizeof(Matrix), &TV, sizeof(Matrix));
		D3D->GetDC()->Unmap(VBuffer, 0);

		D3D->GetDC()->VSSetConstantBuffers(3, 1, &VBuffer);
		D3D->GetDC()->VSSetConstantBuffers(4, 1, &PBuffer);
		D3D->GetDC()->VSSetConstantBuffers(1, 1, &VPBuffer);
		//D3D->GetDC()->GSSetConstantBuffers(0, 1, &VPBuffer);
		D3D->GetDC()->GSSetConstantBuffers(0, 1, &PBuffer);
	}
	{
		Vector4 viewPos = Vector4(GetWorldPos().x, GetWorldPos().y, GetWorldPos().z,1.0f);
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		D3D->GetDC()->Map(viewPosBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		memcpy_s(mappedResource.pData, sizeof(Vector4), &viewPos, sizeof(Vector4));
		D3D->GetDC()->Unmap(viewPosBuffer, 0);
		D3D->GetDC()->PSSetConstantBuffers(0, 1, &viewPosBuffer);
		D3D->GetDC()->GSSetConstantBuffers(1, 1, &viewPosBuffer);
	}
	
	{
		Vector3 viewUp = GetUp();
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		D3D->GetDC()->Map(viewUpBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		memcpy_s(mappedResource.pData, sizeof(Vector4), &viewUp, sizeof(Vector4));
		D3D->GetDC()->Unmap(viewUpBuffer, 0);
		D3D->GetDC()->GSSetConstantBuffers(2, 1, &viewUpBuffer);
	}
	{
		Vector3 viewForward = GetForward();
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		D3D->GetDC()->Map(viewForwardBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		memcpy_s(mappedResource.pData, sizeof(Vector4), &viewForward, sizeof(Vector4));
		D3D->GetDC()->Unmap(viewForwardBuffer, 0);
		D3D->GetDC()->GSSetConstantBuffers(3, 1, &viewForwardBuffer);
	}
    D3D->GetDC()->RSSetViewports(1, viewport.Get11());
}

void Camera::SetShadow()
{
	{
		Matrix TV = view.Transpose();
		Matrix TP = proj.Transpose();
		Matrix TVP = view * proj;
		TVP = TVP.Transpose();
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		D3D->GetDC()->Map(VPBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		memcpy_s(mappedResource.pData, sizeof(Matrix), &TVP, sizeof(Matrix));
		D3D->GetDC()->Unmap(VPBuffer, 0);

		D3D11_MAPPED_SUBRESOURCE mappedResource2;
		D3D->GetDC()->Map(PBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource2);
		memcpy_s(mappedResource2.pData, sizeof(Matrix), &TP, sizeof(Matrix));
		D3D->GetDC()->Unmap(PBuffer, 0);

		D3D11_MAPPED_SUBRESOURCE mappedResource3;
		D3D->GetDC()->Map(VBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource3);
		memcpy_s(mappedResource3.pData, sizeof(Matrix), &TV, sizeof(Matrix));
		D3D->GetDC()->Unmap(VBuffer, 0);

		D3D->GetDC()->VSSetConstantBuffers(3, 1, &VBuffer);
		D3D->GetDC()->VSSetConstantBuffers(4, 1, &PBuffer);
		D3D->GetDC()->VSSetConstantBuffers(1, 1, &VPBuffer);
		D3D->GetDC()->GSSetConstantBuffers(0, 1, &VPBuffer);
	}
	{
		Vector4 viewPos = Vector4(GetWorldPos().x, GetWorldPos().y, GetWorldPos().z, 1.0f);
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		D3D->GetDC()->Map(viewPosBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		memcpy_s(mappedResource.pData, sizeof(Vector4), &viewPos, sizeof(Vector4));
		D3D->GetDC()->Unmap(viewPosBuffer, 0);
		D3D->GetDC()->PSSetConstantBuffers(0, 1, &viewPosBuffer);
		D3D->GetDC()->GSSetConstantBuffers(1, 1, &viewPosBuffer);
	}
	{
		Vector3 viewUp = GetUp();
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		D3D->GetDC()->Map(viewUpBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		memcpy_s(mappedResource.pData, sizeof(Vector4), &viewUp, sizeof(Vector4));
		D3D->GetDC()->Unmap(viewUpBuffer, 0);
		D3D->GetDC()->GSSetConstantBuffers(2, 1, &viewUpBuffer);
	}
	{
		Vector3 viewForward = GetForward();
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		D3D->GetDC()->Map(viewForwardBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		memcpy_s(mappedResource.pData, sizeof(Vector4), &viewForward, sizeof(Vector4));
		D3D->GetDC()->Unmap(viewForwardBuffer, 0);
		D3D->GetDC()->GSSetConstantBuffers(3, 1, &viewForwardBuffer);
	}
	D3D->GetDC()->RSSetViewports(1, viewport.Get11());

	
}

ID3D11Buffer* Camera::VPBuffer = nullptr;
ID3D11Buffer* Camera::VBuffer = nullptr;
ID3D11Buffer* Camera::PBuffer = nullptr;
ID3D11Buffer* Camera::viewPosBuffer = nullptr;
ID3D11Buffer* Camera::viewUpBuffer = nullptr;
ID3D11Buffer* Camera::viewForwardBuffer = nullptr;
Camera* Camera::main = nullptr;
float Camera::mainCamSpeed = 10.0f;

void Camera::CreateStaticMember()
{
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.ByteWidth = sizeof(Matrix);
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//상수버퍼
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		HRESULT hr = D3D->GetDevice()->CreateBuffer(&desc, NULL, &VPBuffer);
		assert(SUCCEEDED(hr));
		
	}
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.ByteWidth = sizeof(Matrix);
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//상수버퍼
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		HRESULT hr = D3D->GetDevice()->CreateBuffer(&desc, NULL, &VBuffer);
		assert(SUCCEEDED(hr));

	}
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.ByteWidth = sizeof(Matrix);
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//상수버퍼
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		HRESULT hr = D3D->GetDevice()->CreateBuffer(&desc, NULL, &PBuffer);
		assert(SUCCEEDED(hr));

	}
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.ByteWidth = sizeof(Vector4);
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//상수버퍼
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		HRESULT hr = D3D->GetDevice()->CreateBuffer(&desc, NULL, &viewPosBuffer);
		assert(SUCCEEDED(hr));
		
	}
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.ByteWidth = sizeof(Vector4);
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//상수버퍼
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		HRESULT hr = D3D->GetDevice()->CreateBuffer(&desc, NULL, &viewUpBuffer);
		assert(SUCCEEDED(hr));

	}
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.ByteWidth = sizeof(Vector4);
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//상수버퍼
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		HRESULT hr = D3D->GetDevice()->CreateBuffer(&desc, NULL, &viewForwardBuffer);
		assert(SUCCEEDED(hr));

	}
	
	
}

void Camera::DeleteStaticMember()
{
    SafeRelease(VPBuffer);
    SafeRelease(VBuffer);
    SafeRelease(PBuffer);
    SafeRelease(viewPosBuffer);
    SafeRelease(viewUpBuffer);
    SafeRelease(viewForwardBuffer);
}

void Camera::ControlMainCam()
{
    if (not main) return;

	if (INPUT->KeyPress('W'))
	{
		//                                  초당100움직임 xyz/s
		main->MoveWorldPos(main->GetForward() * DELTA * mainCamSpeed);
	}
	if (INPUT->KeyPress('S'))
	{
		main->MoveWorldPos(-main->GetForward() * DELTA * mainCamSpeed);
	}
	if (INPUT->KeyPress('A'))
	{
		main->MoveWorldPos(-main->GetRight() * DELTA * mainCamSpeed);
	}
	if (INPUT->KeyPress('D'))
	{
		main->MoveWorldPos(main->GetRight() * DELTA * mainCamSpeed);
	}
	if (INPUT->KeyPress('Q'))
	{
		main->MoveWorldPos(-main->GetUp() * DELTA * mainCamSpeed);
	}
	if (INPUT->KeyPress('E'))
	{
		main->MoveWorldPos(main->GetUp() * DELTA * mainCamSpeed);
	}

	//마우스 우클릭시
	if (INPUT->KeyPress(VK_RBUTTON))
	{
		Vector3 Rot;
		Rot.x = INPUT->movePosition.y * 0.001f;
		Rot.y = INPUT->movePosition.x * 0.001f;
		main->rotation += Rot;
	}
	//휠키로 카메라 앞뒤조절
	main->MoveWorldPos(main->GetForward() * INPUT->wheelMoveValue.z * DELTA);
}
//
bool Camera::Intersect(Vector3 coord)
{
	BoundingFrustum Frustum;

	Frustum.Origin = GetWorldPos();
	Frustum.Orientation = Quaternion::CreateFromRotationMatrix(R);
	Frustum.RightSlope = tanf(fov);
	Frustum.LeftSlope = -Frustum.RightSlope;
	Frustum.TopSlope = Frustum.RightSlope * App.GetHeight() / App.GetWidth();
	Frustum.BottomSlope = -Frustum.TopSlope;
	Frustum.Near = nearZ;
	Frustum.Far = farZ;

	//Frustum.Contains()

	if (Frustum.Contains(coord) != DISJOINT)
	{
		return true;
	}

	return false;
}

bool Camera::Intersect(Collider* target)
{
	BoundingFrustum Frustum;

	Frustum.Origin = GetWorldPos();
	Frustum.Orientation = Quaternion::CreateFromRotationMatrix(R);
	Frustum.RightSlope = tanf(fov);
	Frustum.LeftSlope = -Frustum.RightSlope;
	Frustum.TopSlope = Frustum.RightSlope * App.GetHeight() / App.GetWidth();
	Frustum.BottomSlope = -Frustum.TopSlope;
	Frustum.Near = nearZ;
	Frustum.Far = farZ;


	if (target->type == ColliderType::BOX)
	{
		BoundingBox box2;
		box2.Center = target->GetWorldPos();
		box2.Extents = Vector3(target->S._11, target->S._22, target->S._33);

		if (Frustum.Contains(box2) != DISJOINT)
		{
			return true;
		}
	}
	else if (target->type == ColliderType::OBOX)
	{
		BoundingOrientedBox box2;
		box2.Center = target->GetWorldPos();
		box2.Extents = Vector3(target->S._11, target->S._22, target->S._33);
		box2.Orientation = Quaternion::CreateFromRotationMatrix(target->RT);
		if (Frustum.Contains(box2) != DISJOINT)
		{
			return true;
		}
	}
	else
	{
		BoundingSphere box2;
		box2.Center = target->GetWorldPos();
		box2.Radius = target->S._11;
		if (Frustum.Contains(box2) != DISJOINT)
		{
			return true;
		}
	}

	return false;
}
