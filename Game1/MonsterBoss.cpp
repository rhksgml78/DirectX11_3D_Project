#include "stdafx.h"

void MonsterBoss::Init()
{
	/*
	���� �ݶ��̴� ��ġ
	�ǰݿ� �ݶ��̴� = mixamorig:Hips
	���ݿ� �ݶ��̴�1 = mixamorig:LeftHand
	���ݿ� �ݶ��̴�2 = mixamorig:RightHand
	��ų ���ݿ� �ݶ��̴� = rocks1,2,3->root->Object004

	���� �ִϸ��̼� ����Ʈ
	0. ��Ʈ�� �ƾ��� ��ȿ
	1. �⺻ idle
	2. �ȱ� walk
	3. �ٱ� run
	4. ��ȸ�� left turn
	5. ��ȸ�� right turn
	6. ���� jump
	7. ������ ����
	8. 7������ �̷�
	9. �޼� ����
	10. 9������ �̷�
	11. ȸ�� ����
	12. ����� ��ȿ ���1�ܰ�
	13. ��� 2�ܰ�
	14. �и� ���
	15. �и� �׷α� (�׷α���¿��� �и����� ���� ����)
	16. �и� ���ݳ˹� (�и� �����Ǿ����� 1)
	17. �и� ���ѵ� �Ͼ�� ��� (�и� �����Ǿ����� 2)
	18. �и� �׷α���¿��� �Ͼ�� (�и� ���еǾ�����)
	*/

	root = Actor::Create();
	string file = "BossMonsterSE.xml";
	root->LoadFile(file);
	//float temp = 0.5f;
	//root->Find("Warrok")->material->ambient = Color(temp, temp, temp,1.0f);
	//root->Find("Warrok")->material->diffuse = Color(temp, temp, temp,1.0f);
	//root->Find("Warrok")->material->emissive = Color(0, 0, 0,1.0f);
	original = root->Find("Warrok")->material->ambient;

	// ������ Ÿ���� ����
	root->actortype = Actor::ActorType::BOSS;
	
	bState = Boss_State::NORMAL;

	timer = 0.0f;

	root->anim->ChangeAnimation(AnimationState::LOOP, 1);

	BT = new BehaviorTree();
	BT->Init(this->root);
	BT->board->SetSpd(30.0f); // �ӵ� �ʱ�ȭ
	//BT->board->SetIntroRange(200.0f); // ���ٽ� ��Ʈ�� �׼��� ����
	BT->board->SetRange(200.0f); // Ž�� �� ���� ����
	BT->board->SetMoveRange(150.0f); // �̵������� ����


	// Ʈ����
	trail = new SlashTrail();
	trail->Bottom = root->Find("RightPoint1");
	trail->Top = root->Find("RightPoint2");
	trail->interval = 0.0f;
	trail->maxTrail = 30;
	trail->material->diffuseMap  = RESOURCE->textures.Load("../Texture/slashtrail.png");
	trail->material->diffuse = Color(0.25,0.25,0.25,1.0);
	trailEffect = true;
	/*
	Ʈ������ 2���� ����
	RightPoint1 & RightPoint2
	LeftPoint1 & LeftPoint2
	*/

	// ��Ʈ�� ����
	isintro = false;

	// ��� ���� [12 ��� 1�ܰ�, 13 ��� �Ϸ�ܰ� �ִϸ��̼�]
	deathcount = 0;

	// ����Ʈ ����
	effect = new ExploedEffect();
	effect->init("Boss_Effect");

	deatheffect = new DeathEffect();
	deatheffect->init("Boss_Death_Effect");
	deatheffect->SetRange(Vector3(35.0f, 70.0f, 35.0f));
	deatheffect->SetParticleCount(25);

	roareffect = new RoarEffect();
	roareffect->init("Boss_Roar_Effect");

	// ��ų ����
	rocks1 = new RocksEffect();
	rocks1->Init();
	rocks2 = new RocksEffect();
	rocks2->Init();
	rocks2->root->scale = Vector3(1.5f, 1.0f, 1.5f);

	rocks3 = new RocksEffectVt();
	rocks3->Init();

	// ��ų ���ݽ� ������ �ݶ��̴�
	Skill_Circle_Col = Actor::Create();
	Skill_Circle_Col->LoadFile("Skill_Circle_Col.xml");
	Skill_Circle_Col->root->collider->colOnOff = false;
	Skill_Circle_Col->root->collider->visible = false;
	Skill_Line_Col = Actor::Create();
	Skill_Line_Col->LoadFile("Skill_Line_Col.xml");
	Skill_Line_Col->root->collider->colOnOff = false;
	Skill_Line_Col->root->collider->visible = false;

	//����
	SOUND->AddSound("Boss_Die.wav","DIE");
	SOUND->AddSound("Boss_Jump.wav","JUMP");
	SOUND->AddSound("Slow_Step.wav","SLOWMOVE"); 
	SOUND->AddSound("Fast_Step.wav","FASTMOVE");
	SOUND->AddSound("Boss_Roar.wav","ROAR");
	SOUND->AddSound("MonsterHit.wav","BOSSHIT");

	/*
	�ǰݿ� �ݶ��̴� = mixamorig:Hips
	���ݿ� �ݶ��̴�1 = mixamorig:LeftHand
	���ݿ� �ݶ��̴�2 = mixamorig:RightHand
	*/
	// �ݶ��̴�
	AtkCol1 = root->Find("mixamorig:LeftHand")->collider;
	AtkCol2 = root->Find("mixamorig:RightHand")->collider;
	HitCol = root->Find("mixamorig:Hips")->collider;

	// ������ ���Ͽ�
	// ���ݿ� �ݶ��̴� 
	atkObs = new AttackObserver(root);
	atkObs->name = "BOSS";
	atkSub = new AttackSubject();
	dynamic_cast<AttackSubject*>(this->atkSub)->SetCol(this->root->Find("mixamorig:LeftHand"));
	dynamic_cast<AttackSubject*>(this->atkSub)->SetCol(this->root->Find("mixamorig:RightHand"));
	dynamic_cast<AttackSubject*>(this->atkSub)->SetCol(this->Skill_Circle_Col->root);
	dynamic_cast<AttackSubject*>(this->atkSub)->SetCol(this->Skill_Line_Col->root);
	// �ǰݿ� �ݶ��̴�
	colObs = new CollisionObserver();
	dynamic_cast<CollisionObserver*>(this->colObs)->SetCol(this->root->Find("mixamorig:Hips")->collider);
}

void MonsterBoss::Release()
{
	SafeDelete(colObs);
	SafeDelete(atkSub);
	SafeDelete(atkObs);

	SafeDelete(rocks3);
	SafeDelete(rocks2);
	SafeDelete(rocks1);
	Skill_Line_Col->Release();
	Skill_Circle_Col->Release();
	SafeDelete(roareffect);
	SafeDelete(deatheffect);
	SafeDelete(effect);
	SafeDelete(trail);
	delete BT;
	root->Release();
}

void MonsterBoss::Update()
{
	switch (bState)
	{
	case MonsterBoss::Boss_State::INTRO:
		intro();
		break;
	case MonsterBoss::Boss_State::NORMAL:
		normal();
		break;
	case MonsterBoss::Boss_State::HIT:
		hit();
		break;
	case MonsterBoss::Boss_State::SKILL:
		skill();
		break;
	case MonsterBoss::Boss_State::CHANGE:
		change();
		break;
	case MonsterBoss::Boss_State::PARRY:
		parry();
		break;
	case MonsterBoss::Boss_State::PARRY2:
		parry2();
		break;
	case MonsterBoss::Boss_State::ATTACK:
		attack();
		break;
	case MonsterBoss::Boss_State::DEATH:
		death();
		break;
	default:
		break;
	}

	//�ݶ��̴��� �����ִٸ� �ٽ� ON��Ű�� �Լ�
	if (bState != Boss_State::CHANGE && bState != Boss_State::SKILL && bState != Boss_State::DEATH)
	{
		ColliderOn();
	}

	// 2������� ��ȯ
	if (!ischange && hp <= (maxhp*0.5)) // ü���� ���̵Ǵ� �����̶��
	{
		ischange = true; // ���ǹ��� �ѹ��� ���ð��̱⶧���� ���� �ʱ�ȭX
		bState = Boss_State::CHANGE;
	}

	//�и����Ѹ��
	if (bState != Boss_State::PARRY && INPUT->KeyDown(VK_NUMPAD9))
	{
		bState = Boss_State::PARRY;
	}

	if (isDameged) // ������ �޾����� ���� �����ϵ���
	{
		float temp = RANDOM->Float(0.0f, 1.0f);
		//root->Find("Warrok")->material->ambient = Color(temp, 0.5f, 0.5f, 1.0f);
		root->Find("Warrok")->material->ambient.x = temp;
		if (TIMER->GetTick(Damegedzerotime, 0.5f))
		{
			isDameged = false;
			root->Find("Warrok")->material->ambient = original; // ���������� �ǵ�����
		}
	}


	root->Update();

	if (bState == Boss_State::NORMAL)
	{
		//Ʈ���� ����
		trailAction();
		trail->Update();
	}

	// ����Ʈ ����
	effect->Update();
	deatheffect->Update();
	roareffect->Update();

	// ��ų ����
	if (bState == Boss_State::SKILL)
	{
		if (skilltype == 0)
		{
			power = 2;
			rocks1->Update();
			rocks2->Update();
			Skill_Circle_Col->Update();
		}
		else
		{
			power = 3;
			rocks3->Update();
			Skill_Line_Col->Update();
		}
	}

	// ���������� ����
	atkObs->Update();
	//atkSub->Notify(); //Subject�� �ʿ�ÿ��� Notify�Ͽ� Update����
	colObs->Update();
}

void MonsterBoss::LateUpdate()
{

}

void MonsterBoss::Render()
{

	root->Render();

	if (bState == Boss_State::NORMAL) 
		trail->Render();

	effect->Render();
	deatheffect->Render();
	roareffect->Render();

	// ��ų ����
	if (bState == Boss_State::SKILL )
	{
		if (skilltype == 0)
		{
			rocks1->Render();
			rocks2->Render();
			//Skill_Circle_Col->Render();
		}
		else
		{
			rocks3->Render();
			//Skill_Line_Col->Render();
		}
	}
}

void MonsterBoss::RenderHierarchy()
{
	root->RenderHierarchy();
	roareffect->RenderHierarchy();
	trail->RenderDetail();

	//Skill_Circle_Col->RenderHierarchy();
	//Skill_Line_Col->RenderHierarchy();
	//effect->RenderHierarchy();
	//rocks1->RenderHierarchy();
	//rocks3->RenderHierarchy();
}

void MonsterBoss::trailAction()
{
	// Ʈ���� ����Ʈ
	if (root->anim->GetPlayNum() > 6 && root->anim->GetPlayNum() < 12)
	{
		if (root->anim->GetPlayNum() == 7 || root->anim->GetPlayNum() == 10 || root->anim->GetPlayNum() == 11)
		{
			trail->Bottom = root->Find("RightPoint1");
			trail->Top = root->Find("RightPoint2");
			trailEffect = true;
		}
		if (root->anim->GetPlayNum() == 8 || root->anim->GetPlayNum() == 9)
		{
			trail->Bottom = root->Find("LeftPoint1");
			trail->Top = root->Find("LeftPoint2");
			trailEffect = true;
		}
		if (trailEffect && !trail->isPlaying)
		{
			if (root->anim->GetPlayTime() > 0.3 and root->anim->GetPlayTime() < 0.4)
			{
				trail->Play();
			}
		}
	}
	else
	{
		trailEffect = false;
		trail->Stop();
	}
}

void MonsterBoss::intro()
{
	//��Ʈ��
	if (isintro)
	{
		SOUND->Play("DIE");
		root->anim->ChangeAnimation(AnimationState::ONCE_LAST, 0, 0.01f);
		isintro = false;
	}
	if (!isroar)
	{
		if (root->anim->GetPlayTime() >= 0.4f)
		{
			roareffect->SetPos(root->GetWorldPos());
			roareffect->Play();
			isroar = true;
		}
	}
	if (root->anim->GetPlayTime() >= 1.0f)
	{
		root->anim->ChangeAnimation(AnimationState::LOOP, 1);
		bState = Boss_State::NORMAL;
	}
}

void MonsterBoss::normal()
{
	// Ʈ���� ��� �����϶��� ������Ʈ
	BT->Update();
	//if (root->anim->GetPlayNum() == 2)
	//{
	//	SOUND->Play("SLOWMOVE");
	//}
	//if (root->anim->GetPlayNum() == 3)
	//{
	//	SOUND->Play("FASTMOVE");
	//}
	
	/*
	7. ������ ����
	8. 7������ �̷�
	9. �޼� ����
	10. 9������ �̷�
	11. ȸ�� ����
	*/
	switch (root->anim->GetPlayNum())
	{
	case 2:
		SOUND->Play("SLOWMOVE");
		break;
	case 3:
		SOUND->Play("FASTMOVE");
		break;
	case 7:
		power = 5;
		break;
	case 8:
		power = 5;
		break;
	case 9:
		power = 5;
		break;
	case 10:
		power = 5;
		break;
	case 11:
		power = 10;
		break;
	}
}

void MonsterBoss::hit()
{
	// �ݶ��� ���¿����� �����ߵɵ�?
	// ��Ʈ ���ϸ� ü�� ���ҽ�Ű�� ( ���� 0�̵ɰ�� �ٷ� ������·κ���)
	// �ݶ��� �������ش�.
	// ������Ʈ �κп��� Ÿ�̸Ӹ� ���
	// �ݶ��̴��� ���������϶� �����ð��� �ٽ� ������
}

void MonsterBoss::skill()
{
	if (skilltype == 0)
	{
		if (isskill)
		{
			root->anim->ChangeAnimation(AnimationState::ONCE_LAST, 6, 0.5f);
			isskill = false; // �ִϸ��̼� �ѹ��� �����ϱ� ���� ����
			isstart = false;
		}
		if (!isstart)
		{
			if (root->anim->GetPlayNum() == 6 && root->anim->GetPlayTime() > 0.65f)
			{
				SOUND->Play("JUMP");

				// ���ο� �ܺ��� ����Ʈ ��Ʈ�� ȸ������ ���δٸ��� ����
				rocks1->root->rotation.y = RANDOM->Float(0, PI_2);
				rocks2->root->rotation.y = RANDOM->Float(0, PI_2);

				RockPosSet0();
				skillactive = true;
				rocks1->SetActive(true);
				rocks2->SetActive(true);
				isstart = true;
			}
		}
		if (root->anim->GetPlayNum() == 6 && root->anim->GetPlayTime() >= 1.0f && isstart)
		{
			root->anim->ChangeAnimation(AnimationState::LOOP, 1);
			isskilltonormal = true;
		}
		if (isskilltonormal && rocks1->root->GetWorldPos().y <= rocks1->GetMinPos())
		{
			root->anim->aniScale = 1.3;
			bState = Boss_State::NORMAL;
			skillactive = false;
			isskill = true;
			skilltype = -1;
			isskilltonormal = false;
			Skill_Circle_Col->root->collider->colOnOff = false;
			Skill_Circle_Col->root->collider->visible = false;
		}
	}

	else // ��ų Ÿ���� 1���϶�
	{
		if (isskill)
		{
			root->anim->ChangeAnimation(AnimationState::ONCE_LAST, 6, 0.5f);
			isskill = false; // �ִϸ��̼� �ѹ��� �����ϱ� ���� ����
			isstart = false;
		}
		if (!isstart)
		{
			if (root->anim->GetPlayNum() == 6 && root->anim->GetPlayTime() > 0.65f)
			{
				SOUND->Play("JUMP");

				RockPosSet1();
				skillactive = true;
				rocks3->SetActive(true);
				isstart = true;
			}
		}
		if (root->anim->GetPlayNum() == 6 && root->anim->GetPlayTime() >= 1.0f && isstart)
		{
			root->anim->ChangeAnimation(AnimationState::LOOP, 1);
			isskilltonormal = true;
		}
		if (isskilltonormal && rocks3->rocks[7]->GetWorldPos().y <= rocks3->GetMinPos())
		{
			bState = Boss_State::NORMAL;
			skillactive = false;
			isskill = true;
			skilltype = -1;
			isskilltonormal = false;
			Skill_Line_Col->root->collider->colOnOff = false;
			Skill_Line_Col->root->collider->visible = false;
		}
	}
	
}

void MonsterBoss::change()
{
	if (emissivechange < 1.0f)
	{
		emissivechange += (0.7 * DELTA);
	}
	root->Find("Warrok")->material->emissive = Color(emissivechange, 0, 0,1.0f);
	if (root->Find("Offset")->scale.x < 0.2)
	{
		root->Find("Offset")->scale.x += 0.02* DELTA;
		root->Find("Offset")->scale.y += 0.02* DELTA;
		root->Find("Offset")->scale.z += 0.02* DELTA;
	}

	if (!ischangestart)
	{
		SOUND->Play("ROAR"); // ��ȿ ���� ���
		root->anim->ChangeAnimation(AnimationState::ONCE_LAST, 12); // ��ȿ ��� 1ȸ ����
		root->anim->aniScale = 0.5f;
		ischangestart = true;
	}
	if (ischangestart && root->anim->GetPlayTime() >= 1.0f)
	{
		root->Find("Warrok")->material->emissive = Color(0.7f, 0, 0, 1.0f);
		root->Find("Offset")->scale = Vector3(0.2f, 0.2f, 0.2f); // ������ ũ�� 1.2�� ����
		root->anim->ChangeAnimation(AnimationState::LOOP, 1); // �⺻������� ������
		root->anim->aniScale = 1.3f;
		trail->material->diffuse.x = 1.0f;
		bState = Boss_State::NORMAL; // �ٽ� Ʈ���� �۵��ϵ��� ��ֻ��·� �����ش�.
	}

}

void MonsterBoss::parry()
{
	/*
	�÷��̾ �������϶� ���Ͱ� �����ϰ� ƨ�ܳ�������
	�и����·� �����Ѵ�. (������ �̺�Ʈ ����)

	�и����� ���Խ�
	1. ƨ�ܳ����� ��� ���� (anim play.14 OnceLast)

	2. ƨ�ܳ����� �����ð� �׷α� ���� (anim play.15 Loop)

	3-1. �׷α�����Ͻ� ���� ������ ���
		3-1(1). �÷��̾ �׷α�����϶� ���ݽ� �˹� (anim play.16 OnceLast)
		3-1(2). �Ѿ����� ������ ������ �Ͼ�� (anim play.17 OnceLast)
		3-1(3). ������ ���´� �ٷ� NORMAL�� ����ǰ� Ʈ�� �����

	3-2. �׷α������ ������ �����ð����� ��ġ�Ǿ������
		3-2(1). �׷α���¿��� ���ڸ����� �Ͼ�� ��� ���� (anim play.18 OnceLast)
		3-2(2). �Ͼ�� �ִϸ��̼� �������� ���´� �ٷ� NORMAL�� ����ǰ� Ʈ�� �����

	*/
	//Ÿ�̸�
	if (!parrystart)
	{
		root->anim->ChangeAnimation(AnimationState::ONCE_LAST, 14); // �и����� ��� ����
		parrystart = true;
	}
	if (root->anim->GetPlayNum() == 14 && root->anim->GetPlayTime() >=1.0f)
	{
		root->anim->ChangeAnimation(AnimationState::LOOP, 15); // �׷α� ��� ���� ����
	}

	// �÷��̾ �и��� ����
	if (parrycount < 5) // 1�̵ȼ������� 5�ʱ��� ī��Ʈ
	{
		if (TIMER->GetTick(parrytimer, 1.0f))
		{
			parrycount++; // ī��Ʈ ����
		}
		if (INPUT->KeyDown(VK_SPACE))
		{
			parrycount = 0; // �и� ī��Ʈ �ʱ�ȭ
			parrystart = false;
			bState = Boss_State::PARRY2;
		}
	}
	if (parrycount == 5) // ������ �ð��� �Ǿ��������� �ƹ������� ���ٸ�
	{
		root->anim->ChangeAnimation(AnimationState::ONCE_LAST, 18); // �Ͼ�� ��� ����
		parrycount++; // ���ǹ� Ż�� ��Ű��
	}
	if (root->anim->GetPlayNum() == 18 && root->anim->GetPlayTime() >= 1.0f) // �и����� �����Ű�� ��ַκ���
	{
		parrycount = 0; // �и� ī��Ʈ �ʱ�ȭ
		parrystart = false;
		root->anim->ChangeAnimation(AnimationState::LOOP, 1); // IDLE �ִϸ��̼� ������ �ǵ�����
		bState = Boss_State::NORMAL;
	}
}

void MonsterBoss::parry2()
{
	//Ÿ�̸�
	if (!parrystart2)
	{
		root->anim->ChangeAnimation(AnimationState::ONCE_LAST, 16); // �׷α⿡�� �˹����� ��� ����
		parrystart2 = true;
	}
	if (root->anim->GetPlayNum() == 16 && root->anim->GetPlayTime() >= 1.0f) // �ع� ����� ������
	{
		root->anim->ChangeAnimation(AnimationState::ONCE_LAST, 17); // �ع鿡�� ���ĵ������ ��� ����
	}
	if (root->anim->GetPlayNum() == 17 && root->anim->GetPlayTime() >= 1.0f) // �ع� ����� ������
	{
		parrystart2 = false;
		root->anim->ChangeAnimation(AnimationState::LOOP, 1); // IDLE �ִϸ��̼� ������ �ǵ�����
		bState = Boss_State::NORMAL;
	}

}

void MonsterBoss::attack()
{
	// ������ Ʈ������ �����ϴϱ� ���� �ʿ������
}

void MonsterBoss::death()
{
	// Hit ���� ü���� 0�̵Ǹ� �ٷ� ������·� �ٲ��
	// ����� ����� �ѹ��� �����ϵ���!
	if (!diesound)
	{
		SOUND->Play("DIE");
		diesound = true;
	}
	// �ִϸ��̼��� 1ȸ�� �����Ų��
	//��� ����� 1,2�� ����������.
	if (deathcount == 0)
	{
		//vpop->Play();
		effect->Play();
		effect->SetPos(root->Find("mixamorig:Hips")->GetWorldPos());
		root->anim->aniScale = 1.0f;
		root->anim->ChangeAnimation(AnimationState::ONCE_LAST, 12);
		deathcount++;
	}
	if (root->anim->GetPlayTime() >= 0.5f && deathcount == 1)
	{
		//pop->Play();// ������Ʈ ���
		//effect->Play();
		root->anim->ChangeAnimation(AnimationState::ONCE_LAST, 13);
		deathcount++;
	}
	if (root->anim->GetPlayNum() == 13 && root->anim->GetPlayTime() >= 1.0f
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
	root->Find("Warrok")->material->ambient = Color(deathcolor, deathcolor, deathcolor, 1.0f);
	root->Find("Warrok")->material->diffuse = Color(deathcolor, deathcolor, deathcolor, 1.0f);
	root->Find("Warrok")->material->emissive = Color(deathcolor * 2.0f, 0.0f, 0.0f, 1.0f);
	if (deathcolor <= 0.0f)
	{
		root->visible = false;
	}
}

void MonsterBoss::GetDameged(int damage)
{
	root->Find("mixamorig:Hips")->collider->colOnOff = false;
	if (bState != Boss_State::CHANGE && bState != Boss_State::SKILL && bState != Boss_State::DEATH)
	{
		SOUND->Play("BOSSHIT");
		hp -= damage;
		hitcount++;
		isDameged = true;
		if (hp < 0)
		{
			bState = Boss_State::DEATH;
			hitcount = 0;
		}
		else
		{
			if ((hitcount%4) == 1) // ������ �ǰ� Ƚ�����ٹߵ�
			{
				root->anim->aniScale = 1.0f;

				if (!ischange)
				{
					isskill = true;
					skilltype = 1;
					bState = Boss_State::SKILL;
				}
				else
				{
					isskill = true;
					skilltype = 0;
					bState = Boss_State::SKILL;
				}
			}
		}
	}
}

void MonsterBoss::ColliderOn()
{
	if (GetHitCol()->colOnOff == false)
	{
		if (TIMER->GetTick(colliderzero, 1.0f))
		{
			cout << "������ �ݶ��̴� ON" << endl;
			GetHitCol()->colOnOff = true;
		}
	}
}

void MonsterBoss::RockPosSet0() // 0�� ��ų ����Ʈ ��ġ ����
{
	rocks1->root->SetWorldPosX(root->GetWorldPos().x);
	rocks1->root->SetWorldPosZ(root->GetWorldPos().z);
	rocks2->root->SetWorldPosX(root->GetWorldPos().x);
	rocks2->root->SetWorldPosZ(root->GetWorldPos().z);
	Skill_Circle_Col->root->SetWorldPos(root->GetWorldPos());
	Skill_Circle_Col->root->collider->colOnOff = true;
	Skill_Circle_Col->root->collider->visible = true;
}

void MonsterBoss::RockPosSet1() // 1�� ��ų ����Ʈ ��ġ ����
{
	rocks3->root->rotation.y = root->rotation.y;
	rocks3->root->SetWorldPosX(root->GetWorldPos().x);
	rocks3->root->SetWorldPosZ(root->GetWorldPos().z);
	Skill_Line_Col->root->SetWorldPos(root->GetWorldPos());
	Skill_Line_Col->root->rotation.y = root->rotation.y;
	Skill_Line_Col->root->collider->colOnOff = true;
	Skill_Line_Col->root->collider->visible = true;
}
