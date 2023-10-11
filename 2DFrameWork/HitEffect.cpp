#include "framework.h"

HitEffect::HitEffect()
{
}

HitEffect::~HitEffect()
{
}

void HitEffect::init(string name)
{
	// Ư��ȿ��
	effect = Pop::Create(name); // �Ű����������� ��������, �Ű�����1�̸� ��������
	effect->duration = 0.1f; // ��� ����
	effect->desc.gravity = 10.0f; // Ƣ����� ����(���� ������ ����)
	effect->particleScale = Vector2(1.0f, 1.0f);
	effect->velocityScalar = 50.0f; // Ƣ����� ����
	effect->particleCount = 100; // ������ ����
	effect->scale = Vector3(1.0f, 1.0f, 0.0f);
	effect->material = new Material();
	effect->material->LoadFile("fog.mtl");
}

void HitEffect::Update()
{
	effect->Update();
}

void HitEffect::Render()
{
	effect->Render();
}

void HitEffect::RenderHierarchy()
{
	effect->RenderHierarchy();
}
