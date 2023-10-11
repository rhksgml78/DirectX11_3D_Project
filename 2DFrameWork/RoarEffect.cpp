#include "framework.h"

RoarEffect::RoarEffect()
{
}

RoarEffect::~RoarEffect()
{
}

void RoarEffect::init(string name)
{
	// Ư��ȿ��
	effect = Rain::Create(name); // �Ű����������� ��������, �Ű�����1�̸� ��������
	effect->duration = 4.0f; // ��� ����
	effect->desc.velocity = Vector3(0.0f, 0.0f, 700.0f);
	effect->desc.range = Vector3(500.0f, 500.0f, 500.0f);
	effect->particleScale = Vector2(1.0f, 1.0f);
	effect->particleCount = 1000; // ������ ����
	effect->scale = Vector3(5.0f, 5.0f, 0.0f);
	effect->material = new Material();
	effect->material->LoadFile("roar.mtl");
}

void RoarEffect::Update()
{
	effect->Update();
}

void RoarEffect::Render()
{
	effect->Render();
}

void RoarEffect::RenderHierarchy()
{
	effect->RenderHierarchy();
}
