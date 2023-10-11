#include "framework.h"

IcicleEffect::IcicleEffect()
{
}

IcicleEffect::~IcicleEffect()
{
}

void IcicleEffect::init(string name)
{
	// 특수효과
	effect = Rain::Create(name); // 매개변수없으면 원형폭발, 매개변수1이면 수평퍼짐
	effect->duration = 3.0f; // 재생 길이
	effect->desc.velocity = Vector3(0.0f,-75.0f,0.0f);
	effect->desc.range = Vector3(20.0f,150.0f,20.0f);
	effect->particleScale = Vector2(5.0f,5.0f);
	effect->particleCount = 250; // 파편의 갯수
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
