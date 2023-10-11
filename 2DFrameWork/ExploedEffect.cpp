#include "framework.h"

ExploedEffect::ExploedEffect()
{
}

ExploedEffect::~ExploedEffect()
{
}

void ExploedEffect::init(string name)
{
	// 특수효과
	effect = Pop::Create(name); // 매개변수없으면 원형폭발, 매개변수1이면 수평퍼짐
	effect->SetLocalPosY(10);
	effect->duration = 1.0f; // 재생 길이
	effect->desc.gravity = 0.0f; // 튀어나가는 방향(값이 음수면 위로)
	effect->velocityScalar = 75.0f; // 튀어나가는 강도
	effect->particleCount = 500; // 파편의 갯수
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
