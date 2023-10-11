#include "framework.h"

ExploedEffect::ExploedEffect()
{
}

ExploedEffect::~ExploedEffect()
{
}

void ExploedEffect::init(string name)
{
	// Ư��ȿ��
	effect = Pop::Create(name); // �Ű����������� ��������, �Ű�����1�̸� ��������
	effect->SetLocalPosY(10);
	effect->duration = 1.0f; // ��� ����
	effect->desc.gravity = 0.0f; // Ƣ����� ����(���� ������ ����)
	effect->velocityScalar = 75.0f; // Ƣ����� ����
	effect->particleCount = 500; // ������ ����
	effect->scale = Vector3(1.0f, 1.0f, 0.0f);
	effect->material = new Material();
	effect->material->LoadFile("redsnowflake.mtl");
}

void ExploedEffect::Update()
{
	effect->Update();
}

void ExploedEffect::Render()
{
	effect->Render();
}

void ExploedEffect::RenderHierarchy()
{
	effect->RenderHierarchy();
}
