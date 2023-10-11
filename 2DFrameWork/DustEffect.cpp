#include "framework.h"

DustEffect::DustEffect()
{
}

DustEffect::~DustEffect()
{
}

void DustEffect::init(string name)
{
	// Ư��ȿ��
	effect = Pop::Create(name, 1); // �Ű����������� ��������, �Ű�����1�̸� ��������
	effect->duration = 0.3f; // ��� ����
	effect->desc.gravity = 100.0f; // Ƣ����� ����(���� ������ ����)
	effect->particleScale = Vector2(5.0f, 5.0f);
	effect->velocityScalar = 40.0f; // Ƣ����� ����
	effect->particleCount = 100; // ������ ����
	effect->scale = Vector3(2.0f, 2.0f, 0.0f);
	effect->material = new Material();
	effect->material->LoadFile("fog.mtl");
}

void DustEffect::Update()
{
	effect->Update();
}

void DustEffect::Render()
{
	effect->Render();
}

void DustEffect::RenderHierarchy()
{
	effect->RenderHierarchy();
}
