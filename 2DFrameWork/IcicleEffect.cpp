#include "framework.h"

IcicleEffect::IcicleEffect()
{
}

IcicleEffect::~IcicleEffect()
{
}

void IcicleEffect::init(string name)
{
	// Ư��ȿ��
	effect = Rain::Create(name); // �Ű����������� ��������, �Ű�����1�̸� ��������
	effect->duration = 3.0f; // ��� ����
	effect->desc.velocity = Vector3(0.0f,-75.0f,0.0f);
	effect->desc.range = Vector3(20.0f,150.0f,20.0f);
	effect->particleScale = Vector2(5.0f,5.0f);
	effect->particleCount = 250; // ������ ����
	effect->scale = Vector3(5.0f, 5.0f, 0.0f);
	effect->material = new Material();
	effect->material->LoadFile("icicle.mtl");
}

void IcicleEffect::Update()
{
	effect->Update();
}

void IcicleEffect::Render()
{
	effect->Render();
}

void IcicleEffect::RenderHierarchy()
{
	effect->RenderHierarchy();
	//effect->RenderDetail();
}
