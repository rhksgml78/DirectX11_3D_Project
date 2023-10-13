#include "stdafx.h"

void MonsterBoss::Init()
{
	/*
	보스 콜라이더 위치
	피격용 콜라이더 = mixamorig:Hips
	공격용 콜라이더1 = mixamorig:LeftHand
	공격용 콜라이더2 = mixamorig:RightHand
	스킬 공격용 콜라이더 = rocks1,2,3->root->Object004

	보스 애니매이션 리스트
	0. 인트로 컷씬용 포효
	1. 기본 idle
	2. 걷기 walk
	3. 뛰기 run
	4. 좌회전 left turn
	5. 우회전 right turn
	6. 점프 jump
	7. 오른손 공격
	8. 7번공격 미러
	9. 왼손 공격
	10. 9번공격 미러
	11. 회전 공격
	12. 사망전 포효 사망1단계
	13. 사망 2단계
	14. 패리 충격
	15. 패리 그로기 (그로기상태에서 패리성공 실패 나뉨)
	16. 패리 공격넉백 (패리 성공되었을때 1)
	17. 패리 당한뒤 일어나는 모션 (패리 성공되었을때 2)
	18. 패리 그로기상태에서 일어나기 (패리 실패되었을때)
	*/

	root = Actor::Create();
	string file = "BossMonsterSE.xml";
	root->LoadFile(file);
	//float temp = 0.5f;
	//root->Find("Warrok")->material->ambient = Color(temp, temp, temp,1.0f);
	//root->Find("Warrok")->material->diffuse = Color(temp, temp, temp,1.0f);
	//root->Find("Warrok")->material->emissive = Color(0, 0, 0,1.0f);
	original = root->Find("Warrok")->material->ambient;

	// 액터의 타입을 지정
	root->actortype = Actor::ActorType::BOSS;
	
	bState = Boss_State::NORMAL;

	timer = 0.0f;

	root->anim->ChangeAnimation(AnimationState::LOOP, 1);

	BT = new BehaviorTree();
	BT->Init(this->root);
	BT->board->SetSpd(30.0f); // 속도 초기화
	//BT->board->SetIntroRange(200.0f); // 접근시 인트로 액션할 범위
	BT->board->SetRange(200.0f); // 탐색 및 추적 범위
	BT->board->SetMoveRange(150.0f); // 이동시작할 범위


	// 트레일
	trail = new SlashTrail();
	trail->Bottom = root->Find("RightPoint1");
	trail->Top = root->Find("RightPoint2");
	trail->interval = 0.0f;
	trail->maxTrail = 30;
	trail->material->diffuseMap  = RESOURCE->textures.Load("../Texture/slashtrail.png");
	trail->material->diffuse = Color(0.25,0.25,0.25,1.0);
	trailEffect = true;
	/*
	트레일은 2곳에 있음
	RightPoint1 & RightPoint2
	LeftPoint1 & LeftPoint2
	*/

	// 인트로 관련
	isintro = false;

	// 사망 관련 [12 사망 1단계, 13 사망 완료단계 애니매이션]
	deathcount = 0;

	// 이펙트 관련
	effect = new ExploedEffect();
	effect->init("Boss_Effect");

	deatheffect = new DeathEffect();
	deatheffect->init("Boss_Death_Effect");
	deatheffect->SetRange(Vector3(35.0f, 70.0f, 35.0f));
	deatheffect->SetParticleCount(25);

	roareffect = new RoarEffect();
	roareffect->init("Boss_Roar_Effect");

	// 스킬 관련
	rocks1 = new RocksEffect();
	rocks1->Init();
	rocks2 = new RocksEffect();
	rocks2->Init();
	rocks2->root->scale = Vector3(1.5f, 1.0f, 1.5f);

	rocks3 = new RocksEffectVt();
	rocks3->Init();

	// 스킬 공격시 범위용 콜라이더
	Skill_Circle_Col = Actor::Create();
	Skill_Circle_Col->LoadFile("Skill_Circle_Col.xml");
	Skill_Circle_Col->root->collider->colOnOff = false;
	Skill_Circle_Col->root->collider->visible = false;
	Skill_Line_Col = Actor::Create();
	Skill_Line_Col->LoadFile("Skill_Line_Col.xml");
	Skill_Line_Col->root->collider->colOnOff = false;
	Skill_Line_Col->root->collider->visible = false;

	//사운드
	SOUND->AddSound("Boss_Die.wav","DIE");
	SOUND->AddSound("Boss_Jump.wav","JUMP");
	SOUND->AddSound("Slow_Step.wav","SLOWMOVE"); 
	SOUND->AddSound("Fast_Step.wav","FASTMOVE");
	SOUND->AddSound("Boss_Roar.wav","ROAR");
	SOUND->AddSound("MonsterHit.wav","BOSSHIT");

	/*
	피격용 콜라이더 = mixamorig:Hips
	공격용 콜라이더1 = mixamorig:LeftHand
	공격용 콜라이더2 = mixamorig:RightHand
	*/
	// 콜라이더
	AtkCol1 = root->Find("mixamorig:LeftHand")->collider;
	AtkCol2 = root->Find("mixamorig:RightHand")->collider;
	HitCol = root->Find("mixamorig:Hips")->collider;

	// 옵저버 패턴용
	// 공격용 콜라이더 
	atkObs = new AttackObserver(root);
	atkObs->name = "BOSS";
	atkSub = new AttackSubject();
	dynamic_cast<AttackSubject*>(this->atkSub)->SetCol(this->root->Find("mixamorig:LeftHand"));
	dynamic_cast<AttackSubject*>(this->atkSub)->SetCol(this->root->Find("mixamorig:RightHand"));
	dynamic_cast<AttackSubject*>(this->atkSub)->SetCol(this->Skill_Circle_Col->root);
	dynamic_cast<AttackSubject*>(this->atkSub)->SetCol(this->Skill_Line_Col->root);
	// 피격용 콜라이더
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

	//콜라이더가 꺼져있다면 다시 ON시키는 함수
	if (bState != Boss_State::CHANGE && bState != Boss_State::SKILL && bState != Boss_State::DEATH)
	{
		ColliderOn();
	}

	// 2페이즈로 전환
	if (!ischange && hp <= (maxhp*0.5)) // 체력이 반이되는 조건이라면
	{
		ischange = true; // 조건문에 한번만 들어올것이기때문에 이후 초기화X
		bState = Boss_State::CHANGE;
	}

	//패리당한모션
	if (bState != Boss_State::PARRY && INPUT->KeyDown(VK_NUMPAD9))
	{
		bState = Boss_State::PARRY;
	}

	if (isDameged) // 데미지 받았을때 보스 색변하도록
	{
		float temp = RANDOM->Float(0.0f, 1.0f);
		//root->Find("Warrok")->material->ambient = Color(temp, 0.5f, 0.5f, 1.0f);
		root->Find("Warrok")->material->ambient.x = temp;
		if (TIMER->GetTick(Damegedzerotime, 0.5f))
		{
			isDameged = false;
			root->Find("Warrok")->material->ambient = original; // 원래색으로 되돌리기
		}
	}


	root->Update();

	if (bState == Boss_State::NORMAL)
	{
		//트레일 실행
		trailAction();
		trail->Update();
	}

	// 이펙트 관련
	effect->Update();
	deatheffect->Update();
	roareffect->Update();

	// 스킬 관련
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

	// 옵저버패턴 관련
	atkObs->Update();
	//atkSub->Notify(); //Subject는 필요시에만 Notify하여 Update진행
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

	// 스킬 관련
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
	// 트레일 이펙트
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
	//인트로
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
	// 트리는 노멀 상태일때만 업데이트
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
	7. 오른손 공격
	8. 7번공격 미러
	9. 왼손 공격
	10. 9번공격 미러
	11. 회전 공격
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
	// 콜라이 더온오프를 만들어야될듯?
	// 히트 당하면 체력 감소시키고 ( 만일 0이될경우 바로 사망상태로변경)
	// 콜라어디 오프해준다.
	// 업데이트 부분에서 타이머를 사용
	// 콜라이더가 오프상태일때 일정시간뒤 다시 온으로
}

void MonsterBoss::skill()
{
	if (skilltype == 0)
	{
		if (isskill)
		{
			root->anim->ChangeAnimation(AnimationState::ONCE_LAST, 6, 0.5f);
			isskill = false; // 애니매이션 한번만 실행하기 위한 변수
			isstart = false;
		}
		if (!isstart)
		{
			if (root->anim->GetPlayNum() == 6 && root->anim->GetPlayTime() > 0.65f)
			{
				SOUND->Play("JUMP");

				// 내부와 외부의 이펙트 루트의 회전값을 서로다르게 설정
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

	else // 스킬 타입이 1번일때
	{
		if (isskill)
		{
			root->anim->ChangeAnimation(AnimationState::ONCE_LAST, 6, 0.5f);
			isskill = false; // 애니매이션 한번만 실행하기 위한 변수
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
		SOUND->Play("ROAR"); // 포효 사운드 재생
		root->anim->ChangeAnimation(AnimationState::ONCE_LAST, 12); // 포효 모션 1회 진행
		root->anim->aniScale = 0.5f;
		ischangestart = true;
	}
	if (ischangestart && root->anim->GetPlayTime() >= 1.0f)
	{
		root->Find("Warrok")->material->emissive = Color(0.7f, 0, 0, 1.0f);
		root->Find("Offset")->scale = Vector3(0.2f, 0.2f, 0.2f); // 보스의 크기 1.2로 고정
		root->anim->ChangeAnimation(AnimationState::LOOP, 1); // 기본모션으로 돌리고
		root->anim->aniScale = 1.3f;
		trail->material->diffuse.x = 1.0f;
		bState = Boss_State::NORMAL; // 다시 트리가 작동하도록 노멀상태로 돌려준다.
	}

}

void MonsterBoss::parry()
{
	/*
	플레이어가 방어상태일때 몬스터가 공격하고 튕겨내졌을떄
	패리상태로 진입한다. (씬에서 이벤트 실행)

	패리상태 진입시
	1. 튕겨내지는 모션 실행 (anim play.14 OnceLast)

	2. 튕겨내지고 일정시간 그로기 실행 (anim play.15 Loop)

	3-1. 그로기상태일시 공격 당했을 경우
		3-1(1). 플레이어가 그로기상태일때 공격시 넉백 (anim play.16 OnceLast)
		3-1(2). 넘어진후 보스는 땅에서 일어난다 (anim play.17 OnceLast)
		3-1(3). 보스의 상태는 바로 NORMAL로 변경되고 트리 실행됨

	3-2. 그로기상태의 보스가 일정시간동안 방치되었을경우
		3-2(1). 그로기상태에서 그자리에서 일어나는 모션 실행 (anim play.18 OnceLast)
		3-2(2). 일어나는 애니매이션 종료직후 상태는 바로 NORMAL로 변경되고 트리 실행됨

	*/
	//타이머
	if (!parrystart)
	{
		root->anim->ChangeAnimation(AnimationState::ONCE_LAST, 14); // 패리당한 모션 시작
		parrystart = true;
	}
	if (root->anim->GetPlayNum() == 14 && root->anim->GetPlayTime() >=1.0f)
	{
		root->anim->ChangeAnimation(AnimationState::LOOP, 15); // 그로기 모션 루프 실행
	}

	// 플레이어가 패리에 실패
	if (parrycount < 5) // 1이된순간부터 5초까지 카운트
	{
		if (TIMER->GetTick(parrytimer, 1.0f))
		{
			parrycount++; // 카운트 증가
		}
		if (INPUT->KeyDown(VK_SPACE))
		{
			parrycount = 0; // 패리 카운트 초기화
			parrystart = false;
			bState = Boss_State::PARRY2;
		}
	}
	if (parrycount == 5) // 마지막 시간이 되었을때까지 아무받응이 없다면
	{
		root->anim->ChangeAnimation(AnimationState::ONCE_LAST, 18); // 일어나는 모션 실행
		parrycount++; // 조건문 탈출 시키고
	}
	if (root->anim->GetPlayNum() == 18 && root->anim->GetPlayTime() >= 1.0f) // 패리상태 종료시키고 노멀로변경
	{
		parrycount = 0; // 패리 카운트 초기화
		parrystart = false;
		root->anim->ChangeAnimation(AnimationState::LOOP, 1); // IDLE 애니매이션 루프로 되돌리기
		bState = Boss_State::NORMAL;
	}
}

void MonsterBoss::parry2()
{
	//타이머
	if (!parrystart2)
	{
		root->anim->ChangeAnimation(AnimationState::ONCE_LAST, 16); // 그로기에서 넉백으로 모션 변경
		parrystart2 = true;
	}
	if (root->anim->GetPlayNum() == 16 && root->anim->GetPlayTime() >= 1.0f) // 넥백 모션이 끝나면
	{
		root->anim->ChangeAnimation(AnimationState::ONCE_LAST, 17); // 넥백에서 스탠드업으로 모션 변경
	}
	if (root->anim->GetPlayNum() == 17 && root->anim->GetPlayTime() >= 1.0f) // 넥백 모션이 끝나면
	{
		parrystart2 = false;
		root->anim->ChangeAnimation(AnimationState::LOOP, 1); // IDLE 애니매이션 루프로 되돌리기
		bState = Boss_State::NORMAL;
	}

}

void MonsterBoss::attack()
{
	// 어택은 트리에서 진행하니까 따로 필요없을듯
}

void MonsterBoss::death()
{
	// Hit 이후 체력이 0이되면 바로 사망상태로 바뀌도록
	// 사망시 사운드는 한번만 실행하도록!
	if (!diesound)
	{
		SOUND->Play("DIE");
		diesound = true;
	}
	// 애니매이션은 1회만 재생시킨다
	//사망 모션은 1,2로 나뉘어진다.
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
		//pop->Play();// 팝이펙트 재생
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
			if ((hitcount%4) == 1) // 일정한 피격 횟수마다발동
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
			cout << "보스의 콜라이더 ON" << endl;
			GetHitCol()->colOnOff = true;
		}
	}
}

void MonsterBoss::RockPosSet0() // 0번 스킬 이펙트 위치 세팅
{
	rocks1->root->SetWorldPosX(root->GetWorldPos().x);
	rocks1->root->SetWorldPosZ(root->GetWorldPos().z);
	rocks2->root->SetWorldPosX(root->GetWorldPos().x);
	rocks2->root->SetWorldPosZ(root->GetWorldPos().z);
	Skill_Circle_Col->root->SetWorldPos(root->GetWorldPos());
	Skill_Circle_Col->root->collider->colOnOff = true;
	Skill_Circle_Col->root->collider->visible = true;
}

void MonsterBoss::RockPosSet1() // 1번 스킬 이펙트 위치 세팅
{
	rocks3->root->rotation.y = root->rotation.y;
	rocks3->root->SetWorldPosX(root->GetWorldPos().x);
	rocks3->root->SetWorldPosZ(root->GetWorldPos().z);
	Skill_Line_Col->root->SetWorldPos(root->GetWorldPos());
	Skill_Line_Col->root->rotation.y = root->rotation.y;
	Skill_Line_Col->root->collider->colOnOff = true;
	Skill_Line_Col->root->collider->visible = true;
}
