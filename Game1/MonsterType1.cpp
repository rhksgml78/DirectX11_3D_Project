#include "stdafx.h"

void MonsterType1::Init()
{
	/*
	�ݶ��̴�
	�ǰݿ� �ݶ��̴� : mixamorig:Hips
	���ݿ� �ݶ��̴� 1 : mixamorig:RightFoot
	���ݿ� �ݶ��̴� 2 : mixamorig:LeftHand
	���ݿ� �ݶ��̴� 3 : mixamorig:RightHand
	
	Monster1 �ִϸ��̼� ����Ʈ
	0. ���ٽ� ��ȿ
	1. �⺻ idle
	2. �̵� move
	3. �ǰ� hit
	4. ��� death
	5. ���� ���������
	6. ���� �Ѽ�
	7. ���� ������
	*/
	root = Actor::Create();
	string file = "Monster1SE.xml";
	root->LoadFile(file);
	//float temp = 0.5f;
	//root->Find("PumpkinHulk")->material->ambient = Color(temp, temp, temp, 1.0f);
	//root->Find("PumpkinHulk")->material->diffuse = Color(temp, temp, temp, 1.0f);

	// ������ Ÿ���� ����
	root->actortype = Actor::ActorType::MONSTER;

	// ���� ó�� �����Ҷ� ���ý� ���´� ���Ĺ���
	mState = Monster_State::STANDBY;
	root->anim->ChangeAnimation(AnimationState::LOOP, 1);

	timer = 0.0f;

	BT = new BehaviorTree();
	BT->Init(this->root);
	BT->board->SetSpd(30.0f); // �ӵ� �ʱ�ȭ
	BT->board->SetIntroRange(200.0f); // ���ٽ� ��Ʈ�� �׼��� ����
	BT->board->SetRange(200.0f); // Ž�� �� ���� ����
	BT->board->SetMoveRange(150.0f); // �̵������� ����


	// ��� ���� 
	deathcount = 0;

	// Ʈ����
	trail = new SlashTrail();
	trail->Bottom = root->Find("RightPoint1");
	trail->Top = root->Find("RightPoint2");
	trail->interval = 0.002f;
	trail->maxTrail = 50;
	trail->material->diffuseMap = RESOURCE->textures.Load("../Texture/slashtrail.png");
	trail->material->diffuse = Color(0.25, 0.25, 0.25, 1.0);
	trailEffect = true;
	/*
	Ʈ������ 2���� ����
	RightPoint1 & RightPoint2
	LeftPoint1 & LeftPoint2
	*/

	// ����Ʈ ����
	dusteffect = new DustEffect();
	dusteffect->init("Monster_Type1_dst_Effect");
	isdust = false;

	hiteffect = new HitEffect();
	hiteffect->init("Monster_Type1_hit_Effect");	
	ishit = false;
	
	bloodeffect = new BloodEffect();
	bloodeffect->init("Monster_Type1_Blood_Effect");

	deatheffect = new DeathEffect();
	deatheffect->init("Monster_Type1_Death_Effect");

	// ���� ����
	SOUND->AddSound("Monster1_Die.wav","Monster1_Die");
	SOUND->AddSound("MonsterHit.wav","Monster1_HIT");

	/*
	�ǰݿ� �ݶ��̴� : mixamorig:Hips
	���ݿ� �ݶ��̴� 1 : mixamorig:RightFoot
	���ݿ� �ݶ��̴� 2 : mixamorig:LeftHand
	���ݿ� �ݶ��̴� 3 : mixamorig:RightHand
	*/
	// �ݶ��̴�
	AtkCol1 = root->Find("mixamorig:LeftHand")->collider;
	AtkCol2 = root->Find("mixamorig:RightHand")->collider;
	AtkCol3 = root->Find("mixamorig:RightFoot")->collider;
	HitCol = root->Find("mixamorig:Hips")->collider;
}

void MonsterType1::Release()
{
	SafeDelete(deatheffect);
	SafeDelete(bloodeffect);
	SafeDelete(hiteffect);
	SafeDelete(dusteffect);
	delete BT;
	root->Release();
}

void MonsterType1::Update()
{
	switch (mState)
	{
	case MonsterType1::Monster_State::STANDBY:
		standby();
		break;
	case MonsterType1::Monster_State::NORMAL:
		normal();
		break;
	case MonsterType1::Monster_State::HIT:
		hit();
		break;
	case MonsterType1::Monster_State::DEATH:
		death();
		break;
	}
	// �ݶ��̴��� ��������
	if (!root->Find("mixamorig:Hips")->collider->colOnOff) ColOn();
	
	// ����Ʈ
	if (root->anim->GetPlayNum() == 5) // �ٴ�����ϋ�
	{
		if (root->anim->GetPlayTime() >= 0.35f && root->anim->GetPlayTime() <= 0.36f)
		{
			dusteffect->Play(); // 5�� anim �ٴ� ��� �����
			dusteffect->SetPos(root->GetWorldPos());
			dusteffect->SetPosX(root->GetLocalPos().x + 10); // ����� ��ġ ����
		}
	}

	if (root->anim->GetPlayNum() == 6) // ������ �ֵθ���
	{
		//Ʈ���� ����
		trailAction();
	}

	if (root->anim->GetPlayNum() == 7) // �������϶�
	{
		if (root->anim->GetPlayTime() >= 0.35f && root->anim->GetPlayTime() <= 0.36f)
		{
			hiteffect->Play(); // ������ ����Ʈ
			hiteffect->SetPos(root->GetWorldPos());
			hiteffect->SetPosY(root->GetWorldPos().y + 10.0f); // ��������� ���� ����
			hiteffect->SetPosX(root->GetWorldPos().x + 10.0f); // ��������� ���� ����
		}
	}

	root->Update();
	trail->Update();

	deatheffect->Update();
	bloodeffect->Update();
	dusteffect->Update();
	hiteffect->Update();
}

void MonsterType1::LateUpdate()
{

}

void MonsterType1::Render()
{
	root->Render();
	trail->Render();
	deatheffect->Render();
	bloodeffect->Render();
	dusteffect->Render();
	hiteffect->Render();
}

void MonsterType1::RenderHierarchy()
{
	root->RenderHierarchy();
	//bloodeffect->RenderHierarchy();
	//dusteffect->RenderHierarchy();
	//hiteffect->RenderHierarchy();
	//trail->RenderDetail();
}

void MonsterType1::trailAction()
{
	// Ʈ���� ����Ʈ
	trail->Bottom = root->Find("RightPoint1");
	trail->Top = root->Find("RightPoint2");
	if (root->anim->GetPlayTime() > 0.3 && trailEffect)
	{
		trail->Play();
		trailEffect = false;
	}
	if (root->anim->GetPlayTime() >= 0.7f && !trailEffect)
	{
		trailEffect = true;
		trail->Stop();
	}
}

void MonsterType1::standby() // ���� ó�� �����Ҷ� ���Ĺ��� ���¿��� �ִϸ��̼� �ѹ����
{
	if (mState == Monster_State::STANDBY && isstandby)
	{
		// ��� �� ��ٷ� ���¸� �����ϵ����Ѵ�.
		if (root->anim->GetPlayTime() >= 1.0f)
		{
			isstandby = false;
			mState = Monster_State::NORMAL;
		}
	}
}

void MonsterType1::normal() // �⺻�����ϋ��� Ʈ���� ������Ʈ�Ѵ�.
{
	BT->Update();
	SetPower(); // ��ǿ����� ���ݷ� ���� �Լ�
}

void MonsterType1::hit() // �ܺο��� �ǰݴ��������� �����»���, �ѹ������ NORMAL���·� ����ȴ�
{
	if (!is_hit)
	{
		root->anim->aniScale = 1.5f; // �ǰݸ�� ������ ���� ����������
		SOUND->Play("Monster1_HIT");
		root->Find("mixamorig:Hips")->collider->colOnOff = false;
		bloodeffect->Play();
		bloodeffect->SetPos(root->GetWorldPos());
		int temp = RANDOM->Int(7,14);
		bloodeffect->SetPosY(temp);
		if (hp <= 0)
		{
			hp = 0;
			mState = Monster_State::DEATH;
		}
		else
		{
			root->anim->ChangeAnimation(AnimationState::ONCE_LAST, 3);
			is_hit = true;
		}
	}
	// ��� �� ��ٷ� ���¸� �����ϵ����Ѵ�.
	if (root->anim->GetPlayTime() >= 1.0f && is_hit)
	{
		root->anim->aniScale = 1.0f; // ��� �ӵ� ������� �ǵ�����
		mState = Monster_State::NORMAL;
		root->anim->ChangeAnimation(AnimationState::LOOP, 1);
		is_hit = false;
	}
}

void MonsterType1::death() // �ܺο��� ����������� ���Խ� ����
{
	if (mState == Monster_State::DEATH && deathcount == 0)
	{
		SOUND->Play("Monster1_Die");
		root->anim->ChangeAnimation(AnimationState::ONCE_LAST, 4);
		deathcount++;
	}
	if (root->anim->GetPlayNum() == 4 && root->anim->GetPlayTime() >= 1.0f
		&& deathcolor > 0.0f)
	{
		deathcolor -= 0.25f * DELTA;
		if (!deatheffect->GetPlay())
		{
			deatheffect->SetPos(root->Find("mixamorig:Hips")->GetWorldPos());
			deatheffect->SetPosY(deatheffect->GetHalfY());
			deatheffect->Play();
		}
	}
	root->Find("PumpkinHulk")->material->ambient = Color(deathcolor, deathcolor, deathcolor, 1.0f);
	root->Find("PumpkinHulk")->material->diffuse = Color(deathcolor, deathcolor, deathcolor, 1.0f);
	if (deathcolor <= 0.0f)
	{
		root->visible = false;
	}
}

void MonsterType1::GetDameged(int damage)
{
	root->Find("mixamorig:Hips")->collider->colOnOff = false;
	hp -= damage;
	mState = Monster_State::HIT; // �ǰݴ��ϸ� �ٷ� ��Ʈ��������
}

void MonsterType1::SetPower()
{
	/*
	5. ���� ���������
	6. ���� �Ѽ�
	7. ���� ������
	*/
	switch (root->anim->GetPlayNum())
	{
	case 5: // ���� ���������
		power = 10;
		break;
	case 6: // ���� �Ѽ�
		power = 5;
		break;
	case 7: // ���� ������
		power = 7;
		break;
	}
}

void MonsterType1::ColOn()
{
	if (mState != Monster_State::DEATH) // ���Ͱ� ������°� �ƴ϶��
	{
		// �ǰݿ� �ݶ��̴��� �������ֵ��� �Ѵ�.
		if (root->Find("mixamorig:Hips")->collider->colOnOff == false)
		{
			// �ݶ��̴��� ���� �ִٸ� Ÿ�̸� �۵�
			if (TIMER->GetTick(coliderzerotime, 1.0f))// 1�ʵڿ�
			{
				cout << "Monster Type1 HitCol ON !" << endl;
				root->Find("mixamorig:Hips")->collider->colOnOff = true;
			}
		}
	}
}