#include "framework.h"

FireEffect::FireEffect()
{
}

FireEffect::~FireEffect()
{
}

void FireEffect::init(string name)
{
	// 특수효과
	effect = Rain::Create(name); // 매개변수없으면 원형폭발, 매개변수1이면 수평퍼짐
	effect->duration = 2.0f; // 재생 길이
	effect->desc.velocity = Vector3(0.0f, 30.0f, 0.0f);
	effect->desc.range = Vector3(30.0f, 15.0f, 30.0f);
	effect->particleScale = Vector2(5.0f, 5.0f);
	effect->particleCount = 250; // 파편의 갯수
	effect->scale = Vector3(5.0f, 5.0f, 0.0f);
	effect->material = new Material();
	effect->material->LoadFile("fire.mtl");
}

void FireEffect::Update()
{
	effect->Update();
}

void FireEffect::Render()
{
	effect->Render();
}

void FireEffect::RenderHierarchy()
{
	effect->RenderHierarchy();
	//effect->RenderDetail();
}
