#include "stdafx.h"

RocksEffectVt::RocksEffectVt()
{
}

RocksEffectVt::~RocksEffectVt()
{
}

void RocksEffectVt::Init()
{
	root = Actor::Create("Rocks_Effect_Vertical_Type");
	root->SetWorldPosY(-50);
	// 스킬 관련
	for (int i = 0; i < MAXROCK; i++)
	{
		rocks[i] = Actor::Create();
		rocks[i]->LoadFile("Sharp_Rock3.xml");
		rocks[i]->name = "RocksVt" + to_string(i + 1);
		rocks[i]->SetLocalPosZ(i * 20); // 일자로 앞으로 있는 것
		rocks[i]->Find("Offset")->scale.y = 1 + (0.1*i);
		root->AddChild(rocks[i]);

		pop[i] = new ExploedEffect();
		pop[i]->init("RockVt_Pop" + to_string(i));
		pop[i]->SetGravity(50.0f);
		pop[i]->SetParticleCount(200);
		pop[i]->SetVelocityScalar(150);
	}
}

void RocksEffectVt::Release()
{
}

void RocksEffectVt::Update()
{
	EmissiveChange();
	Action();

	root->Update();
	for (int i = 0; i < MAXROCK; i++)
	{
		pop[i]->Update();
	}
}

void RocksEffectVt::Render()
{
	root->Render();
	for (int i = 0; i < MAXROCK; i++)
	{
		pop[i]->Render();
	}
}

void RocksEffectVt::RenderHierarchy()
{
	root->RenderHierarchy();
	for (int i = 0; i < MAXROCK; i++)
	{
		pop[i]->RenderHierarchy();
	}
}

void RocksEffectVt::Action()
{

	if (isActive)
	{

		if (isPop)
		{
			for (int i = 0; i < MAXROCK; i++)
			{
				pop[i]->SetPosX(rocks[i]->Find("Object004")->GetWorldPos().x);
				pop[i]->SetPosZ(rocks[i]->Find("Object004")->GetWorldPos().z);
			}
			isPop = false;
		}

		for (int i = 0; i < MAXROCK; i++)
		{
			pop[i]->SetPosX(rocks[i]->Find("Object004")->GetWorldPos().x);
			pop[i]->SetPosZ(rocks[i]->Find("Object004")->GetWorldPos().z);
		}

		if (skillcount < 8)
		{
			if (TIMER->GetTick(skillcountzero, 0.1f))
			{
				isskill[skillcount] = true;
				pop[skillcount]->Play();

				++skillcount;
				if (skillcount == 8)
				{
					islast = true;
				}
			}
		}
		for (int i = 0; i < MAXROCK; i++)
		{
			if (isskill[i] && rocks[i]->GetWorldPos().y < maxpos)
				rocks[i]->MoveWorldPos(UP * upspeed * DELTA);
			else if (rocks[i]->GetWorldPos().y > maxpos)
				isskill[i] = false;

			if (!isskill[i] && rocks[i]->GetWorldPos().y > minpos)
				rocks[i]->MoveWorldPos(-UP * (downspeed*1.5f) * DELTA);
		}

		if (islast && !isskill[7] && rocks[7]->GetWorldPos().y <= minpos)
		{
			for (int i = 0; i < MAXROCK; i++)
			{
				rocks[i]->SetWorldPosY(minpos);
			}
			islast = false;
			isActive = false;
			skillcount = 0;
		}
	}
}

void RocksEffectVt::EmissiveChange()
{
	if (TIMER->GetTick(emzrotime, 0.1f))
	{
		for (int i = 0; i < MAXROCK; i++)
		{
			if (rocks[i]->GetWorldPos().y > -10)
			{
				float temp = RANDOM->Float(0.0f, 1.0f);
				rocks[i]->Find("Object004")->material->emissive = Color(temp, 0.0f, 0.0f, 1.0f);
			}
			else
			{
				rocks[i]->Find("Object004")->material->emissive = Color(0.0f, 0.0f, 0.0f, 1.0f);
			}
		}
	}
}
