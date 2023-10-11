#include "framework.h"

void Particle::UpdateParticle()
{
	if (isPlaying)
	{
		playTime += DELTA;
		if (playTime > duration)
		{
			Stop();
		}
	}
}

void Particle::Gui()
{
	if (ImGui::Button("Play"))
	{
		Play();
	}
	ImGui::SameLine();
	if (ImGui::Button("Stop"))
	{
		Stop();
	}
	//���� ��� �ð�
	ImGui::Text("Playtime : %f", PlayTime());
	//�� ����� �ð�
	ImGui::SliderFloat("duration", &duration, 0.0f, 100.0f);
}
ID3D11Buffer* Rain::RainBuffer = nullptr;
void Rain::CreateStaticMember()
{
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.ByteWidth = sizeof(RAIN_DESC);
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//�������
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		HRESULT hr = D3D->GetDevice()->CreateBuffer(&desc, NULL, &RainBuffer);
		assert(SUCCEEDED(hr));

	}
}

void Rain::DeleteStaticMember()
{
	SafeRelease(RainBuffer);
}

Rain* Rain::Create(string name)
{
	Rain* temp = new Rain();
	temp->name = name;

	temp->mesh = make_shared<Mesh>();
	temp->mesh->LoadFile("7.Billboard.mesh");
	temp->shader = RESOURCE->shaders.Load("7.Rain.hlsl");
	temp->shader->LoadGeometry();
	temp->type = ObType::Rain;
	//temp->visible = false;

	return temp;
}

void Rain::Render()
{
	desc.time = TIMER->GetWorldTime();
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		D3D->GetDC()->Map(RainBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		memcpy_s(mappedResource.pData, sizeof(RAIN_DESC), &desc,
			sizeof(RAIN_DESC));
		D3D->GetDC()->Unmap(RainBuffer, 0);
		D3D->GetDC()->VSSetConstantBuffers(10, 1, &RainBuffer);
	}
	if (isPlaying)
		Actor::Render();
}

void Rain::Update()
{
	Particle::UpdateParticle();
	Actor::Update();
}



void Rain::Reset()
{
	delete[](VertexPS*)mesh->vertices;
	delete[] mesh->indices;
	mesh->vertices = new VertexPS[particleCount];
	mesh->indices = new UINT[particleCount];
	mesh->vertexCount = particleCount;
	mesh->indexCount = particleCount;

	Vector2 scale;


	for (UINT i = 0; i < particleCount; i++)
	{
		//�̹��� ũ�� ���μ��θ� ������
		//4~8 ���̰�

		//������
		scale.x = RANDOM->Float(-particleScale.x, particleScale.x);
		scale.y = RANDOM->Float(-particleScale.y, particleScale.y);
		scale.x = S._11 + scale.x;
		scale.y = S._22 + scale.y;
		if (scale.x < 1.0f)scale.x = 1.0f;
		if (scale.y < 1.0f)scale.y = 1.0f;

		Vector3 position;
		//��������ġ   //-4~8   ~ 4~ 8
		position.x = RANDOM->Float(-desc.range.x, desc.range.x);
		position.y = RANDOM->Float(-desc.range.y, desc.range.y);
		position.z = RANDOM->Float(-desc.range.z, desc.range.z);

		((VertexPS*)mesh->vertices)[i].position = position;
		((VertexPS*)mesh->vertices)[i].size = scale;
		mesh->indices[i] = i;
	}
	SafeRelease(mesh->vertexBuffer);
	SafeRelease(mesh->indexBuffer);

	//CreateVertexBuffer
	{
		D3D11_BUFFER_DESC desc;
		desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(VertexPS) * particleCount;
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

void Rain::Play()
{
	Reset();
	Particle::Play();
}

void Rain::Stop()
{
	Particle::Stop();
}


void Rain::RenderDetail()
{
	Actor::RenderDetail();
	if (ImGui::BeginTabBar("MyTabBar3"))
	{
		if (ImGui::BeginTabItem("Rain"))
		{
			Particle::Gui();
			ImGui::SliderFloat3("Velocity", (float*)&desc.velocity, -1000, 1000);
			ImGui::SliderFloat3("particleRange", (float*)&desc.range, 0, 1000);
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