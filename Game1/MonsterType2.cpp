#include "stdafx.h"

void MonsterType2::Init()
{
	/*
	콜라이더
	피격용 콜라이더 : mixamorig:Hips
	공격용 콜라이더1 : Fire_Effect_Collider
	공격용 콜라이더2 : Icicle_Effect_Collider
	
	Monster2 파일의 애니매이션 순서
	0. 기본상태 idle
	1. 이동 move
	2. 피격 hit
	3. 사망스타일1 왼쪽으로넘어지며 사망
	4. 사망스타일2 앞으로쓰러지며 사망
	5. 공격스타일1 전방공격 - 투사체 발사?
	6. 공격스타일2 땅에서위로올라가는공격 - 불꽃 범위 공격
	7. 공격스타일3 위에서아래로떨어지는공격 - 고드름 공격
	*/
	root = Actor::Create();
	string file = "Monster2.xml";
	root->LoadFile(file);
	//float temp = 0.5f;
	//root->Find("Nightshade")->material->ambient = Color(temp, temp, temp, 1.0f);
	//root->Find("Nightshade")->material->diffuse = Color(temp, temp, temp, 1.0f);

	// 액터의 타입을 지정
	root->actortype = Actor::ActorType::MONSTER2;

	// 몬스터 처음 시작할때 세팅시 상태는 스탠바이
	mState = Monster_State::STANDBY;
	root->anim->ChangeAnimation(AnimationState::LOOP, 0);

	timer = 0.0f;

	BT = new BehaviorTree();
	BT->Init(this->root);
	BT->board->SetSpd(20.0f); // 속도 초기화
	BT->board->SetRange(200.0f); // 탐색 및 추적 범위
	BT->board->SetMoveRange(150.0f); // 이동시작할 범위


	// 사망 관련 
	deathcount = 0;

	// 이펙트 관련

	iceeffect = new IcicleEffect();
	iceeffect->init("Monster_Type2_ice_Effect");

	fireeffect = new FireEffect();
	fireeffect->init("Monster_Type2_fire_Effect");

	bloodeffect = new BloodEffect();
	bloodeffect->init("Monster_Type2_Blood_Effect");

	deatheffect = new DeathEffect();
	deatheffect->init("Monster_Type2_Death_Effect");

	// 스킬이펙트 액터는 루트에 AddChild 하지 않는다
	FireCol = Actor::Create("Fire_Effect_Collider");
	FireCol->LoadFile("Fire_Effect_Collider.xml");
	
	IcicleCol = Actor::Create("Icicle_Effect_Collider");
	IcicleCol->LoadFile("Icicle_Effect_Collider.xml");


	// 사운드 관련
	SOUND->AddSound("Monster2_Die.wav","Monster2_Die");
	SOUND->AddSound("Monster2_Fire.wav","Monster2_Fire");
	SOUND->AddSound("Monster2_Ice.wav","Monster2_Ice");
	SOUND->AddSound("MonsterHit.wav","Monster2_Hit");

	/*
	피격용 콜라이더 : mixamorig:Hips
	공격용 콜라이더1 : Fire_Effect_Collider
	공격용 콜라이더2 : Icicle_Effect_Collider
	*/
	// 콜라이더
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
	// 콜라이더가 꺼졌을때
	if (!root->Find("mixamorig:Hips")->collider->colOnOff) ColOn();

#pragma region Skill_Action
	// 스킬발동 타이밍 & 스킬 이팩트가 완료되었을때 콜라이더 끄기
	if (root->anim->GetPlayNum() == 6)
	{

		power = 5;
		if (root->anim->GetPlayTime() >= 0.5 && fireeffect->GetPlay() == false)
		{
			SOUND->Play("Monster2_Fire");
			AtkCol1 = FireCol->collider;
			fireeffect->Play(); // 6번 anim 불
			fireeffect->SetPos(BT->board->GetObj()->root->GetWorldPos());
			FireCol->SetWorldPos(BT->board->GetObj()->root->GetWorldPos());
			FireCol->SetWorldPosY(BT->board->GetObj()->root->GetWorldPos().y + 5);
			FireCol->collider->colOnOff = true;
		}
	}
	if (!fireeffect->GetPlay()) // 이펙트실행이 끝나면 콜라이더 끄기
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
			iceeffect->Play(); // 7번 anim 고드름
			iceeffect->SetPos(BT->board->GetObj()->root->GetWorldPos());
			IcicleCol->SetWorldPos(BT->board->GetObj()->root->GetWorldPos());
			IcicleCol->SetWorldPosY(BT->board->GetObj()->root->GetWorldPos().y + 5);
			IcicleCol->collider->colOnOff = true;
		}
	}
	if (!iceeffect->GetPlay()) // 이펙트실행이 끝나면 콜라이더 끄기
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

void MonsterType2::standby() // 스탠바이는 처음에 한번만 실행하고 다시안들어올조건
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

void MonsterType2::normal() // 기본상태일떄는 트리를 업데이트한다.
{
	BT->Update();
}

void MonsterType2::hit() // 외부에서 피격당했을때에 들어오는상태, 한번실행뒤 NORMAL상태로 변경된다
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
	// 재생 후 곧바로 상태를 변경하도록한다.
	if (root->anim->GetPlayTime() >= 1.0f && is_hit)
	{
		mState = Monster_State::NORMAL;
		root->anim->ChangeAnimation(AnimationState::LOOP, 0);
		is_hit = false;
	}
}

void MonsterType2::death() // 외부에서 사망조건으로 진입시 실행
{
	if (mState == Monster_State::DEATH && deathcount == 0)
	{
		SOUND->Play("Monster2_Die");
		int temp = RANDOM->Int(3, 4); // 사망모션 2개중 한개로 랜덤!
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
	mState = Monster_State::HIT; // 피격당하면 바로 히트판정으로
}

void MonsterType2::ColOn()
{
	if (mState != Monster_State::DEATH) // 몬스터가 사망상태가 아니라면
	{
		// 콜라이더가 꺼져 있다면 타이머 작동
		if (TIMER->GetTick(coliderzerotime, 1.0f))// 1초뒤에
		{
			cout << "Monster Type2 HitCol ON !" << endl;
			root->Find("mixamorig:Hips")->collider->colOnOff = true;
		}
	}
}
