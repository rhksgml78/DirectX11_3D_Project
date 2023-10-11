#include "framework.h"

RoarEffect::RoarEffect()
{
}

RoarEffect::~RoarEffect()
{
}

void RoarEffect::init(string name)
{
	// 특수효과
	effect = Rain::Create(name); // 매개변수없으면 원형폭발, 매개변수1이면 수평퍼짐
	effect->duration = 4.0f; // 재생 길이
	effect->desc.velocity = Vector3(0.0f, 0.0f, 700.0f);
	effect->desc.range = Vector3(500.0f, 500.0f, 500.0f);
	effect->particleScale = Vector2(1.0f, 1.0f);
	effect->particleCount = 1000; // 파편의 갯수
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
