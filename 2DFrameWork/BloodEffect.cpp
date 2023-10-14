#include "framework.h"

BloodEffect::BloodEffect()
{
}

BloodEffect::~BloodEffect()
{
}

void BloodEffect::init(string name)
{
	// 특수효과
	effect = Pop::Create(name); // 매개변수없으면 원형폭발, 매개변수1이면 수평퍼짐
	effect->duration = 0.2f; // 재생 길이
	effect->desc.gravity = 0.0f; // 튀어나가는 방향(값이 음수면 위로)
	effect->particleScale = Vector2(7.0f, 7.0f);
	effect->velocityScalar = 50.0f; // 튀어나가는 강도
	effect->particleCount = 100; // 파편의 갯수
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
