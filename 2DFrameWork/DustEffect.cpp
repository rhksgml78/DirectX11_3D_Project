#include "framework.h"

DustEffect::DustEffect()
{
}

DustEffect::~DustEffect()
{
}

void DustEffect::init(string name)
{
	// 특수효과
	effect = Pop::Create(name, 1); // 매개변수없으면 원형폭발, 매개변수1이면 수평퍼짐
	effect->duration = 0.3f; // 재생 길이
	effect->desc.gravity = 100.0f; // 튀어나가는 방향(값이 음수면 위로)
	effect->particleScale = Vector2(5.0f, 5.0f);
	effect->velocityScalar = 40.0f; // 튀어나가는 강도
	effect->particleCount = 100; // 파편의 갯수
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
