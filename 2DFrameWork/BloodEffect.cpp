#include "framework.h"

BloodEffect::BloodEffect()
{
}

BloodEffect::~BloodEffect()
{
}

void BloodEffect::init(string name)
{
	// Ư��ȿ��
	effect = Pop::Create(name); // �Ű����������� ��������, �Ű�����1�̸� ��������
	effect->duration = 0.2f; // ��� ����
	effect->desc.gravity = 0.0f; // Ƣ����� ����(���� ������ ����)
	effect->particleScale = Vector2(7.0f, 7.0f);
	effect->velocityScalar = 50.0f; // Ƣ����� ����
	effect->particleCount = 100; // ������ ����
	effect->scale = Vector3(1.0f, 1.0f, 0.0f);
	effect->material = new Material();
	effect->material->LoadFile("blood.mtl");
}

void BloodEffect::Update()
{
	effect->Update();
}

void BloodEffect::Render()
{
	effect->Render();
}

void BloodEffect::RenderHierarchy()
{
	effect->RenderHierarchy();
}
