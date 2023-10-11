#include "framework.h"

DeathEffect::DeathEffect()
{
}

DeathEffect::~DeathEffect()
{
}

void DeathEffect::init(string name)
{
	// 특수효과
	effect = Rain::Create(name); // 매개변수없으면 원형폭발, 매개변수1이면 수평퍼짐
	effect->duration = 2.0f; // 재생 길이
	effect->desc.velocity = Vector3(0.0f, 60.0f, 0.0f);
	effect->desc.range = Vector3(20.0f, 40.0f, 20.0f);
	effect->particleScale = Vector2(0.0f, 0.0f);
	effect->particleCount = 20; // 파편의 갯수
	effect->scale = Vector3(2.0f, 2.0f, 0.0f);
	effect->material = new Material();
	effect->material->LoadFile("death.mtl");
	effect->material->diffuseMap->LoadFile("dust.png");
}

void DeathEffect::Update()
{
	effect->Update();
}

void DeathEffect::Render()
{
	effect->Render();
}

void DeathEffect::RenderHierarchy()
{
	effect->RenderHierarchy();
}
