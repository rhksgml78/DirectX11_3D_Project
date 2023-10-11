#include "stdafx.h"

void MonsterType1::Init()
{
	/*
	콜라이더
	피격용 콜라이더 : mixamorig:Hips
	공격용 콜라이더 1 : mixamorig:RightFoot
	공격용 콜라이더 2 : mixamorig:LeftHand
	공격용 콜라이더 3 : mixamorig:RightHand
	
	Monster1 애니매이션 리스트
	0. 접근시 포효
	1. 기본 idle
	2. 이동 move
	3. 피격 hit
	4. 사망 death
	5. 공격 양손찍기공격
	6. 공격 한손
	7. 공격 발차기
	*/
	root = Actor::Create();
	string file = "Monster1SE.xml";
	root->LoadFile(file);
	//float temp = 0.5f;
	//root->Find("PumpkinHulk")->material->ambient = Color(temp, temp, temp, 1.0f);
	//root->Find("PumpkinHulk")->material->diffuse = Color(temp, temp, temp, 1.0f);

	// 액터의 타입을 지정
	root->actortype = Actor::ActorType::MONSTER;

	// 몬스터 처음 시작할때 세팅시 상태는 스탠바이
	mState = Monster_State::STANDBY;
	root->anim->ChangeAnimation(AnimationState::LOOP, 1);

	timer = 0.0f;

	BT = new BehaviorTree();
	BT->Init(this->root);
	BT->board->SetSpd(30.0f); // 속도 초기화
	BT->board->SetIntroRange(200.0f); // 접근시 인트로 액션할 범위
	BT->board->SetRange(200.0f); // 탐색 및 추적 범위
	BT->board->SetMoveRange(150.0f); // 이동시작할 범위


	// 사망 관련 
	deathcount = 0;

	// 트레일
	trail = new SlashTrail();
	trail->Bottom = root->Find("RightPoint1");
	trail->Top = root->Find("RightPoint2");
	trail->interval = 0.002f;
	trail->maxTrail = 50;
	trail->material->diffuseMap = RESOURCE->textures.Load("../Texture/slashtrail.png");
	trail->material->diffuse = Color(0.25, 0.25, 0.25, 1.0);
	trailEffect = true;
	/*
	트레일은 2곳에 있음
	RightPoint1 & RightPoint2
	LeftPoint1 & LeftPoint2
	*/

	// 이펙트 관련
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

	// 사운드 관련
	SOUND->AddSound("Monster1_Die.wav","Monster1_Die");
	SOUND->AddSound("MonsterHit.wav","Monster1_HIT");

	/*
	피격용 콜라이더 : mixamorig:Hips
	공격용 콜라이더 1 : mixamorig:RightFoot
	공격용 콜라이더 2 : mixamorig:LeftHand
	공격용 콜라이더 3 : mixamorig:RightHand
	*/
	// 콜라이더
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
	// 콜라이더가 꺼졌을때
	if (!root->Find("mixamorig:Hips")->collider->colOnOff) ColOn();
	
	// 이펙트
	if (root->anim->GetPlayNum() == 5) // 바닥찍기일떄
	{
		if (root->anim->GetPlayTime() >= 0.35f && root->anim->GetPlayTime() <= 0.36f)
		{
			dusteffect->Play(); // 5번 anim 바닥 찍기 충격파
			dusteffect->SetPos(root->GetWorldPos());
			dusteffect->SetPosX(root->GetLocalPos().x + 10); // 충격파 위치 조절
		}
	}

	if (root->anim->GetPlayNum() == 6) // 오른손 휘두르기
	{
		//트레일 실행
		trailAction();
	}

	if (root->anim->GetPlayNum() == 7) // 발차기일때
	{
		if (root->anim->GetPlayTime() >= 0.35f && root->anim->GetPlayTime() <= 0.36f)
		{
			hiteffect->Play(); // 발차기 이펙트
			hiteffect->SetPos(root->GetWorldPos());
			hiteffect->SetPosY(root->GetWorldPos().y + 10.0f); // 발차기높이 조금 조절
			hiteffect->SetPosX(root->GetWorldPos().x + 10.0f); // 발차기높이 조금 조절
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
	// 트레일 이펙트
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

void MonsterType1::standby() // 몬스터 처음 시작할때 스탠바이 상태에서 애니매이션 한번재생
{
	if (mState == Monster_State::STANDBY && isstandby)
	{
		// 재생 후 곧바로 상태를 변경하도록한다.
		if (root->anim->GetPlayTime() >= 1.0f)
		{
			isstandby = false;
			mState = Monster_State::NORMAL;
		}
	}
}

void MonsterType1::normal() // 기본상태일떄는 트리를 업데이트한다.
{
	BT->Update();
	SetPower(); // 모션에따른 공격력 세팅 함수
}

void MonsterType1::hit() // 외부에서 피격당했을때에 들어오는상태, 한번실행뒤 NORMAL상태로 변경된다
{
	if (!is_hit)
	{
		root->anim->aniScale = 1.5f; // 피격모션 느려서 조금 빠르게진행
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
	// 재생 후 곧바로 상태를 변경하도록한다.
	if (root->anim->GetPlayTime() >= 1.0f && is_hit)
	{
		root->anim->aniScale = 1.0f; // 모션 속도 원래대로 되돌리기
		mState = Monster_State::NORMAL;
		root->anim->ChangeAnimation(AnimationState::LOOP, 1);
		is_hit = false;
	}
}

void MonsterType1::death() // 외부에서 사망조건으로 진입시 실행
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
	mState = Monster_State::HIT; // 피격당하면 바로 히트판정으로
}

void MonsterType1::SetPower()
{
	/*
	5. 공격 양손찍기공격
	6. 공격 한손
	7. 공격 발차기
	*/
	switch (root->anim->GetPlayNum())
	{
	case 5: // 공격 양손찍기공격
		power = 10;
		break;
	case 6: // 공격 한손
		power = 5;
		break;
	case 7: // 공격 발차기
		power = 7;
		break;
	}
}

void MonsterType1::ColOn()
{
	if (mState != Monster_State::DEATH) // 몬스터가 사망상태가 아니라면
	{
		// 피격용 콜라이더만 관리해주도록 한다.
		if (root->Find("mixamorig:Hips")->collider->colOnOff == false)
		{
			// 콜라이더가 꺼져 있다면 타이머 작동
			if (TIMER->GetTick(coliderzerotime, 1.0f))// 1초뒤에
			{
				cout << "Monster Type1 HitCol ON !" << endl;
				root->Find("mixamorig:Hips")->collider->colOnOff = true;
			}
		}
	}
}