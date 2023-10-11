#include "stdafx.h"

void MonsterType2::Init()
{
	/*
	�ݶ��̴�
	�ǰݿ� �ݶ��̴� : mixamorig:Hips
	���ݿ� �ݶ��̴�1 : Fire_Effect_Collider
	���ݿ� �ݶ��̴�2 : Icicle_Effect_Collider
	
	Monster2 ������ �ִϸ��̼� ����
	0. �⺻���� idle
	1. �̵� move
	2. �ǰ� hit
	3. �����Ÿ��1 �������γѾ����� ���
	4. �����Ÿ��2 �����ξ������� ���
	5. ���ݽ�Ÿ��1 ������� - ����ü �߻�?
	6. ���ݽ�Ÿ��2 ���������οö󰡴°��� - �Ҳ� ���� ����
	7. ���ݽ�Ÿ��3 �������Ʒ��ζ������°��� - ��帧 ����
	*/
	root = Actor::Create();
	string file = "Monster2.xml";
	root->LoadFile(file);
	//float temp = 0.5f;
	//root->Find("Nightshade")->material->ambient = Color(temp, temp, temp, 1.0f);
	//root->Find("Nightshade")->material->diffuse = Color(temp, temp, temp, 1.0f);

	// ������ Ÿ���� ����
	root->actortype = Actor::ActorType::MONSTER2;

	// ���� ó�� �����Ҷ� ���ý� ���´� ���Ĺ���
	mState = Monster_State::STANDBY;
	root->anim->ChangeAnimation(AnimationState::LOOP, 0);

	timer = 0.0f;

	BT = new BehaviorTree();
	BT->Init(this->root);
	BT->board->SetSpd(20.0f); // �ӵ� �ʱ�ȭ
	BT->board->SetRange(200.0f); // Ž�� �� ���� ����
	BT->board->SetMoveRange(150.0f); // �̵������� ����


	// ��� ���� 
	deathcount = 0;

	// ����Ʈ ����

	iceeffect = new IcicleEffect();
	iceeffect->init("Monster_Type2_ice_Effect");

	fireeffect = new FireEffect();
	fireeffect->init("Monster_Type2_fire_Effect");

	bloodeffect = new BloodEffect();
	bloodeffect->init("Monster_Type2_Blood_Effect");

	deatheffect = new DeathEffect();
	deatheffect->init("Monster_Type2_Death_Effect");

	// ��ų����Ʈ ���ʹ� ��Ʈ�� AddChild ���� �ʴ´�
	FireCol = Actor::Create("Fire_Effect_Collider");
	FireCol->LoadFile("Fire_Effect_Collider.xml");
	
	IcicleCol = Actor::Create("Icicle_Effect_Collider");
	IcicleCol->LoadFile("Icicle_Effect_Collider.xml");


	// ���� ����
	SOUND->AddSound("Monster2_Die.wav","Monster2_Die");
	SOUND->AddSound("Monster2_Fire.wav","Monster2_Fire");
	SOUND->AddSound("Monster2_Ice.wav","Monster2_Ice");
	SOUND->AddSound("MonsterHit.wav","Monster2_Hit");

	/*
	�ǰݿ� �ݶ��̴� : mixamorig:Hips
	���ݿ� �ݶ��̴�1 : Fire_Effect_Collider
	���ݿ� �ݶ��̴�2 : Icicle_Effect_Collider
	*/
	// �ݶ��̴�
	AtkCol1 = FireCol->collider;
	AtkCol2 = IcicleCol->collider;
	HitCol = root->Find("mixamorig:Hips")->collider;
}

void MonsterType2::Release()
{
	IcicleCol->Release();
	FireCol->Release();
	SafeDelete(deatheffect);
	SafeDelete(bloodeffect);
	SafeDelete(fireeffect);
	SafeDelete(iceeffect);
	delete BT;
	root->Release();

}

void MonsterType2::Update()
{
	switch (mState)
	{
	case MonsterType2::Monster_State::STANDBY:
		standby();
		break;
	case MonsterType2::Monster_State::NORMAL:
		normal();
		break;
	case MonsterType2::Monster_State::HIT:
		hit();
		break;
	case MonsterType2::Monster_State::DEATH:
		death();
		break;
	}
	// �ݶ��̴��� ��������
	if (!root->Find("mixamorig:Hips")->collider->colOnOff) ColOn();

#pragma region Skill_Action
	// ��ų�ߵ� Ÿ�̹� & ��ų ����Ʈ�� �Ϸ�Ǿ����� �ݶ��̴� ����
	if (root->anim->GetPlayNum() == 6)
	{

		power = 5;
		if (root->anim->GetPlayTime() >= 0.5 && fireeffect->GetPlay() == false)
		{
			SOUND->Play("Monster2_Fire");
			AtkCol1 = FireCol->collider;
			fireeffect->Play(); // 6�� anim ��
			fireeffect->SetPos(BT->board->GetObj()->root->GetWorldPos());
			FireCol->SetWorldPos(BT->board->GetObj()->root->GetWorldPos());
			FireCol->SetWorldPosY(BT->board->GetObj()->root->GetWorldPos().y + 5);
			FireCol->collider->colOnOff = true;
		}
	}
	if (!fireeffect->GetPlay()) // ����Ʈ������ ������ �ݶ��̴� ����
	{
		FireCol->collider->colOnOff = false;
	}

	if (root->anim->GetPlayNum() == 7)
	{
		power = 3;
		if (root->anim->GetPlayTime() >= 0.5 && iceeffect->GetPlay() == false)
		{
			SOUND->Play("Monster2_Ice");
			AtkCol2 = IcicleCol->collider;
			iceeffect->Play(); // 7�� anim ��帧
			iceeffect->SetPos(BT->board->GetObj()->root->GetWorldPos());
			IcicleCol->SetWorldPos(BT->board->GetObj()->root->GetWorldPos());
			IcicleCol->SetWorldPosY(BT->board->GetObj()->root->GetWorldPos().y + 5);
			IcicleCol->collider->colOnOff = true;
		}
	}
	if (!iceeffect->GetPlay()) // ����Ʈ������ ������ �ݶ��̴� ����
	{
		IcicleCol->collider->colOnOff = false;
	}
#pragma endregion

	root->Update();
	FireCol->Update();
	IcicleCol->Update();

	deatheffect->Update();
	bloodeffect->Update();
	iceeffect->Update();
	fireeffect->Update();
}

void MonsterType2::LateUpdate()
{

}

void MonsterType2::Render()
{
	root->Render();
	deatheffect->Render();
	bloodeffect->Render();
	iceeffect->Render();
	fireeffect->Render();
	//FireCol->Render();
	//IcicleCol->Render();
}

void MonsterType2::RenderHierarchy()
{
	root->RenderHierarchy();
	//deatheffect->RenderHierarchy();
	//bloodeffect->RenderHierarchy();
	//fireeffect->RenderHierarchy();
	//iceeffect->RenderHierarchy();
	//FireCol->RenderHierarchy();
	//IcicleCol->RenderHierarchy();
}

void MonsterType2::standby() // ���Ĺ��̴� ó���� �ѹ��� �����ϰ� �ٽþȵ�������
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

void MonsterType2::normal() // �⺻�����ϋ��� Ʈ���� ������Ʈ�Ѵ�.
{
	BT->Update();
}

void MonsterType2::hit() // �ܺο��� �ǰݴ��������� �����»���, �ѹ������ NORMAL���·� ����ȴ�
{
	if (!is_hit)
	{
		SOUND->Play("Monster2_Hit");
		root->Find("mixamorig:Hips")->collider->colOnOff = false;
		bloodeffect->Play();
		bloodeffect->SetPos(root->GetWorldPos());
		int temp = RANDOM->Int(7, 14);
		bloodeffect->SetPosY(temp);
		if (hp <= 0)
		{
			hp = 0;
			mState = Monster_State::DEATH;
		}
		else
		{
			root->anim->ChangeAnimation(AnimationState::ONCE_LAST, 2);
			is_hit = true;
		}
	}
	// ��� �� ��ٷ� ���¸� �����ϵ����Ѵ�.
	if (root->anim->GetPlayTime() >= 1.0f && is_hit)
	{
		mState = Monster_State::NORMAL;
		root->anim->ChangeAnimation(AnimationState::LOOP, 0);
		is_hit = false;
	}
}

void MonsterType2::death() // �ܺο��� ����������� ���Խ� ����
{
	if (mState == Monster_State::DEATH && deathcount == 0)
	{
		SOUND->Play("Monster2_Die");
		int temp = RANDOM->Int(3, 4); // ������ 2���� �Ѱ��� ����!
		if (temp == 3)
		{
			root->anim->ChangeAnimation(AnimationState::ONCE_LAST, temp);
		}
		else
		{
			root->anim->ChangeAnimation(AnimationState::ONCE_LAST, temp);
		}
		deathcount++;
	}
	if ((root->anim->GetPlayNum() == 3 || root->anim->GetPlayNum() == 4) && 
		root->anim->GetPlayTime() >= 1.0f && deathcolor > 0.0f)
	{
		deathcolor -= 0.25f * DELTA;
		if (!deatheffect->GetPlay())
		{
			deatheffect->SetPos(root->Find("mixamorig:Hips")->GetWorldPos());
			deatheffect->SetPosY(deatheffect->GetHalfY());
			deatheffect->Play();
		}
	}
	root->Find("Nightshade")->material->ambient = Color(deathcolor, deathcolor, deathcolor, 1.0f);
	root->Find("Nightshade")->material->diffuse = Color(deathcolor, deathcolor, deathcolor, 1.0f);
	if (deathcolor <= 0.0f)
	{
		root->visible = false;
	}
}

void MonsterType2::GetDameged(int damage)
{
	root->Find("mixamorig:Hips")->collider->colOnOff = false;
	hp -= damage;
	mState = Monster_State::HIT; // �ǰݴ��ϸ� �ٷ� ��Ʈ��������
}

void MonsterType2::ColOn()
{
	if (mState != Monster_State::DEATH) // ���Ͱ� ������°� �ƴ϶��
	{
		// �ݶ��̴��� ���� �ִٸ� Ÿ�̸� �۵�
		if (TIMER->GetTick(coliderzerotime, 1.0f))// 1�ʵڿ�
		{
			cout << "Monster Type2 HitCol ON !" << endl;
			root->Find("mixamorig:Hips")->collider->colOnOff = true;
		}
	}
}
