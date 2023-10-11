#include "stdafx.h"

RocksEffect::RocksEffect()
{
}

RocksEffect::~RocksEffect()
{
}

void RocksEffect::Init()
{
	root = Actor::Create("Rocks_Effect_Circle_Type");
	root->SetWorldPosY(-50);
	// 스킬 관련
	for (int i = 0; i < MAXROCK; i++)
	{
		rocks[i] = Actor::Create();
		rocks[i]->LoadFile("Sharp_Rock2.xml");
		rocks[i]->name = "Rocks" + to_string(i+1);
		rocks[i]->rotation.y = (0 + ((360/ MAXROCK) *i))*TORADIAN;
		rocks[i]->Find("Offset")->scale.y = RANDOM->Float(1.0f, 1.5f);
		root->AddChild(rocks[i]);

		pop[i] = new ExploedEffect();
		pop[i]->init("Rock_Pop" + to_string(i + 1));
		pop[i]->SetGravity(50.0f);
		pop[i]->SetParticleCount(200);
		pop[i]->SetVelocityScalar(150);
	}
}

void RocksEffect::Release()
{
}

void RocksEffect::Update()
{
	EmissiveChange();
	Action();

	root->Update();
	for (int i = 0; i < MAXROCK; i++)
	{
		pop[i]->Update();
	}
}

void RocksEffect::Render()
{
	root->Render();
	for (int i = 0; i < MAXROCK; i++)
	{
		pop[i]->Render();
	}
}

void RocksEffect::RenderHierarchy()
{
	root->RenderHierarchy();
	for (int i = 0; i < MAXROCK; i++)
	{
		pop[i]->RenderHierarchy();
	}
}

void RocksEffect::Action()
{
	if (isActive && isMoveUp)
	{
		if (isPop)
		{
			for (int i = 0; i < MAXROCK; i++)
			{
				pop[i]->SetPosX(rocks[i]->Find("Object004")->GetWorldPos().x);
				pop[i]->SetPosZ(rocks[i]->Find("Object004")->GetWorldPos().z);
				pop[i]->Play();
			}
			isPop = false;
		}
		if (root->GetWorldPos().y < maxpos)
		{
			root->MoveWorldPos(UP * upspeed * DELTA);
		}
		if (root->GetWorldPos().y >= maxpos)
		{
			isMoveUp = false;
		}
	}
	if (!isMoveUp)
	{
		if (root->GetWorldPos().y > minpos)
		{
			root->MoveWorldPos(-UP * downspeed * DELTA);
		}
		if (root->GetWorldPos().y <= minpos)
		{
			isMoveUp = true;
			isActive = false;
			isPop = true;
			root->SetWorldPosY(minpos);
		}
	}

}

void RocksEffect::EmissiveChange()
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
