#include "framework.h"
ID3D11Buffer* Pop::PopBuffer = nullptr;
void Pop::CreateStaticMember()
{
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.ByteWidth = sizeof(POP_DESC);
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//�������
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		HRESULT hr = D3D->GetDevice()->CreateBuffer(&desc, NULL, &PopBuffer);
		assert(SUCCEEDED(hr));
	}
}

void Pop::DeleteStaticMember()
{
	SafeRelease(PopBuffer);
}

Pop* Pop::Create(string name)
{
	Pop* temp = new Pop();
	temp->name = name;
	temp->mesh = make_shared<Mesh>();
	temp->mesh->LoadFile("8.Billboard.mesh");
	temp->shader = RESOURCE->shaders.Load("8.Pop.hlsl");
	temp->shader->LoadGeometry();
	temp->type = ObType::Pop;
	return temp;
}

Pop* Pop::Create(string name, int num)
{
	Pop* temp = new Pop();
	temp->name = name+to_string(num);
	temp->mesh = make_shared<Mesh>();
	temp->mesh->LoadFile("8.Billboard.mesh");
	temp->shader = RESOURCE->shaders.Load("8.VerticalPop.hlsl");
	temp->shader->LoadGeometry();
	temp->type = ObType::Pop;
	return temp;
}

void Pop::Render()
{
	desc.duration = duration;
	//����� ������ �ð�
	desc.time = playTime;
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		D3D->GetDC()->Map(PopBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		memcpy_s(mappedResource.pData, sizeof(POP_DESC), &desc,
			sizeof(POP_DESC));
		D3D->GetDC()->Unmap(PopBuffer, 0);
		D3D->GetDC()->VSSetConstantBuffers(10, 1, &PopBuffer);
	}
	if (isPlaying)
		Actor::Render();
}

void Pop::Update()
{
	Particle::UpdateParticle();
	Actor::Update();
}

void Pop::Reset()
{
	delete[](VertexPSV*)mesh->vertices;
	delete[] mesh->indices;
	mesh->vertices = new VertexPSV[particleCount];
	mesh->indices = new UINT[particleCount];
	mesh->vertexCount = particleCount;
	mesh->indexCount = particleCount;

	Vector2 scale;


	for (UINT i = 0; i < particleCount; i++)
	{
		//�̹��� ũ�� ���μ��θ� ������
		//4~8 ���̰�
		scale.x = RANDOM->Float(-particleScale.x, particleScale.x);
		scale.y = RANDOM->Float(-particleScale.y, particleScale.y);
		scale.x = S._11 + scale.x;
		scale.y = S._22 + scale.y;
		if (scale.x < 1.0f)scale.x = 1.0f;
		if (scale.y < 1.0f)scale.y = 1.0f;

		Vector3 position = Vector3(0, 0, 0);


		//���⺤�� Right
		Vector3 velocity = Vector3(1, 0, 0);

		//������ ȸ���� 3����
		Vector3 rot;
		rot.x = RANDOM->Float(0.0f, PI * 2.0f);
		rot.y = RANDOM->Float(0.0f, PI * 2.0f);
		rot.z = RANDOM->Float(0.0f, PI * 2.0f);

		//������ ȸ�����
		Matrix matRot = Matrix::CreateFromYawPitchRoll(rot.y, rot.x, rot.z);
		// v = v * R
		velocity = Vector3::TransformNormal(velocity, matRot);
		velocity *= velocityScalar;

		//���� ���⺤�͸� 3������ ���������� ȸ������ ��´�.
		((VertexPSV*)mesh->vertices)[i].velocity = velocity;
		((VertexPSV*)mesh->vertices)[i].position = position;
		((VertexPSV*)mesh->vertices)[i].size = scale;
		mesh->indices[i] = i;
	}
	SafeRelease(mesh->vertexBuffer);
	SafeRelease(mesh->indexBuffer);

	//CreateVertexBuffer
	{
		D3D11_BUFFER_DESC desc;
		desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(VertexPSV) * particleCount;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = mesh->vertices;

		HRESULT hr = D3D->GetDevice()->CreateBuffer(&desc, &data, &mesh->vertexBuffer);
		assert(SUCCEEDED(hr));
	}

	//Create Index Buffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(UINT) * particleCount;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = mesh->indices;

		HRESULT hr = D3D->GetDevice()->CreateBuffer(&desc, &data, &mesh->indexBuffer);
		assert(SUCCEEDED(hr));
	}
}

void Pop::Play()
{
	Reset();
	Particle::Play();
}

void Pop::Stop()
{
	Particle::Stop();
}


void Pop::RenderDetail()
{
	Actor::RenderDetail();
	if (ImGui::BeginTabBar("MyTabBar3"))
	{
		if (ImGui::BeginTabItem("Pop"))
		{
			Particle::Gui();
			ImGui::SliderFloat("gravity", &desc.gravity, -100.0f, 100.0f);
			ImGui::SliderFloat("velocityScalar", &velocityScalar, 0.0f, 1000.0f);
			ImGui::SliderFloat2("particleScale", (float*)&particleScale, 0, 100);
			ImGui::SliderInt("particleCount", &particleCount, 1, 100);

			if (ImGui::Button("Reset"))
			{
				Reset();
			}
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
}

