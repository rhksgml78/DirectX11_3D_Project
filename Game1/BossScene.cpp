#include "stdafx.h"

// 기본 생성자 소멸자
BossScene::BossScene() { }
BossScene::~BossScene() { }

void BossScene::Init() // 초기화
{
	cam = Camera::Create();
	//cam->LoadFile("BossSceneCam.xml");
	cam->LoadFile("PPTCAMPOS.xml");
	Camera::main = cam;

	sky = Sky::Create(); // 현재 default = yokohama.dds
	sky->texCube->LoadFile("sunsetcube1024.dds");

	map = Terrain::Create();
	//map->LoadFile("Terrain.xml");
	map->LoadFile("DJ_Terrain.xml");
	map->showNode = true;
	map->UpdateMeshUv();
	map->CreateStructuredBuffer();
	//그림자가 표현될 수치 적용시키기
	map->material->shadow = 0.5f; // 변도없을예정인값
	map->showNode = false;

	for (int i = 0; i < 3; i++) // 임시적으로 장애물 3개 배치할것입니다.
	{
		map_stone[i] = Actor::Create("Rock" + to_string(i + 1));
		string temp = "MapRock" + to_string(i + 1) + ".xml";
		map_stone[i]->LoadFile(temp);
	}

	//임시 플레이어 - 현재 단순한 움직임만 가능하며 카메라모션은 모두 주석처리상태
	player = new APlayer();
	player->Init();

	gameui = GameUI::Create();

	// 보스
	boss = new MonsterBoss();
	boss->Init();
	boss->root->SetWorldPos(Vector3(0,0,-250));
	boss->GetBoard()->SetObj(player->root); // 트리 동작용 세팅
	boss->GetBoard()->SetTerrain(map); // 트리 동작용 세팅
	player->SetTarget(boss->root);

	// 테스트용 몬스터
	monster = new MonsterType1();
	monster->Init();
	monster->SetPos(Vector3(50, 0, -100));
	monster->GetBoard()->SetObj(player->root);
	monster->GetBoard()->SetTerrain(map);

	monster2 = new MonsterType2();
	monster2->Init();
	monster2->SetPos(Vector3(-50, 0, -100));
	monster2->GetBoard()->SetObj(player->root);
	monster2->GetBoard()->SetTerrain(map);

	// 그림자 2,4 그림자맵핑용 쉐이더파일 로드
	shadow = new Shadow();
	RESOURCE->shaders.Load("4.ShadowMap.hlsl");
	RESOURCE->shaders.Load("2.ShadowMap.hlsl");

	// 출력 화면 크기 조절용
	ResizeScreen();

	// 컷씬 관련
	isintro = false;
	Camera::mainCamSpeed = 100.0f;
}

void BossScene::Release()
{
	// 해제 순서는 할당순서의 반대
	SafeDelete(shadow);

	// 테스트용 몬스터
	SafeDelete(monster);
	SafeDelete(monster2);

	SafeDelete(boss);
	SafeDelete(player);

	gameui->Release();

	for (int i = 0; i < 3; i++)
		map_stone[i]->Release();

	map->Release();
	sky->Release();
	cam->Release();
}
void BossScene::Hi() // GUI 디테일 확인
{
	shadow->RenderDetail();
	if (ImGui::Begin("Hierarchy"))
	{
		Camera::main->RenderHierarchy();
		//gameui->RenderHierarchy();
		for (int i = 0; i < 3; i++)
			map_stone[i]->RenderHierarchy();

		sky->RenderHierarchy();
		map->RenderHierarchy();
		boss->RenderHierarchy();
		player->RenderHierarchy();
		monster->RenderHierarchy();
		monster2->RenderHierarchy();
		ImGui::End();
	}
}
void BossScene::CamSetPlayer() // 플레이어의 CAM효과를 처리하지않으므로 정해진 위치로 고정
{
	cam->SetWorldPos(player->root->Find("Campoint")->GetWorldPos());
	cam->rotation.y = player->root->Find("Offset")->rotation.y - 180*TORADIAN;
}

void BossScene::Update()
{
	for (int i = 0; i < 3; i++) // 임시적으로 장애물 3개 배치할것입니다.
	{
		Vector3 loadpos;
		loadpos = map->root->Find("COL" + to_string(i + 1))->GetWorldPos();
		map_stone[i]->SetWorldPos(loadpos);
	}

	// 씬의 주된 업데이트 내용들
	Camera::ControlMainCam(); // 카메라만 따로 움직이고싶다면 주석해제
	ImGui::SliderFloat("CAMSPEED",&Camera::mainCamSpeed, 0.0f, 100.0f);
	//if (CamisPlayer) CamSetPlayer(); // 컷씬이아닐떄는 카메라 플레이어에 고정
	//if (!iscutscene) CutScene(); // 컷씬이 한번도 실행안했다면 계속 실행

	// 플레이어 Y축 계속틀어짐. 임시용으로 변경 플레이어 담당측 문제 파악 필요
	if (player->root->GetLocalPos().y != 0.0f)
		player->root->SetLocalPosY(0);


	// 피격에 따른 동작체크용 임시 명령
	if (INPUT->KeyDown(VK_NUMPAD4))
	{
		boss->GetDameged(10);
	}

	Hi(); // 하이어라이키

	// 업데이트 일괄 실행
	Camera::main->Update();
	sky->Update();
	map->Update();
	player->Update();
	boss->Update();
	monster ->Update();
	monster2->Update();
	gameui->Update(player);
	gameui->BossBarUpdate(boss);

	//for (int i = 0; i < 3; i++)
	//	map_stone[i]->Update();
}

void BossScene::LateUpdate()
{
	// 충돌검사나 상호작용등은 Late에서 진행
	// 현재 충돌검사는 차후 옵저버 패턴 채용시 적용할 예정

#pragma region Player Attacks
	if (PlayerAttaks()) // 플레이어의 모션이 공격관련 모션일때
	{
		if (boss->GetHitCol()->colOnOff) // 보스의 피격 콜라이더가 켜져 있을때
		{
			if (player->GetAktCol1()->Intersect(boss->GetHitCol())) // 공격콜라이더 충돌시
			{
				//cout << "보스 에게 " << player->GetPower() << " 의 데미지!" << endl;
				boss->GetDameged(player->GetPower());
			}
		}

		if (monster->GetHitCol()->colOnOff) // 몬스터 타입1의 피격 콜라이더가 켜져 있을때
		{
			if (player->GetAktCol1()->Intersect(monster->GetHitCol())) // 공격콜라이더 충돌시
			{
				//cout << "몬스터 타입1 에게 " << player->GetPower() << " 의 데미지!" << endl;
				monster->GetDameged(player->GetPower());
			}
		}

		if (monster2->GetHitCol()->colOnOff) // 몬스터 타입2의 피격 콜라이더가 켜져 있을때
		{
			if (player->GetAktCol1()->Intersect(monster2->GetHitCol())) // 공격콜라이더 충돌시
			{
				//cout << "몬스터 타입2 에게 " << player->GetPower() << " 의 데미지!" << endl;
				monster2->GetDameged(player->GetPower());
			}
		}
	}
#pragma endregion

#pragma region Boss Attacks
	//if (BossAttacks()) // 보스의 모션이 공격관련 모션일때
	//{ 
	//	if (player->GetHitCol()->colOnOff && // 플레이어의 피격 콜라이더가 켜져있고
	//		player->root->anim->GetPlayNum() != 27) // 구르기상태가 아닐때
	//	{
	//		if (boss->GetAktCol1()->Intersect(player->GetHitCol()))
	//		{
	//			//cout << "플레이어 에게 " << boss->GetPower() << " 의 데미지!" << endl;
	//			player->GetDameged(boss->GetPower());
	//		}
	//		if (boss->GetAktCol2()->Intersect(player->GetHitCol()))
	//		{
	//			//cout << "플레이어 에게 " << boss->GetPower() << " 의 데미지!" << endl;
	//			player->GetDameged(boss->GetPower());
	//		}
	//	}
	//}

	//if (BossSkillAttacks()) // 보스가 스킬 시전중일때
	//{
	//	if (player->GetHitCol()->colOnOff && // 플레이어의 피격 콜라이더가 켜져있고
	//		player->root->anim->GetPlayNum() != 27) // 구르기상태가 아닐때
	//	{
	//		if (boss->GetSkillCol1()->Intersect(player->GetHitCol()))
	//		{
	//			//cout<<"플레이어 에게 " << boss->GetPower() << " 의 데미지!" << endl;
	//			player->GetDameged(boss->GetPower());
	//		}
	//		if (boss->GetSkillCol2()->Intersect(player->GetHitCol()))
	//		{
	//			//cout << "플레이어 에게 " << boss->GetPower() << " 의 데미지!" << endl;
	//			player->GetDameged(boss->GetPower());
	//		}
	//	}
	//}
#pragma endregion

#pragma region Monster Attacks
	//if (MonsterAttacks()) // 몬스터 1번타입이 공격상태일때
	//{
	//	if (player->GetHitCol()->colOnOff && // 플레이어의 피격 콜라이더가 켜져있고
	//		player->root->anim->GetPlayNum() != 27) // 구르기상태가 아닐때
	//	{
	//		// 몬스터의 공격 콜라이더들이 플레이어의 히트콜라이더에 충돌했을때
	//		// 몬스터1번 타입의 콜라이더는 왼손, 오른손, 발 3개이다!
	//		if (monster->GetAktCol1()->Intersect(player->GetHitCol()) ||
	//			monster->GetAktCol2()->Intersect(player->GetHitCol()) ||
	//			monster->GetAktCol3()->Intersect(player->GetHitCol()))
	//		{
	//			cout << "플레이어 에게 [몬스터 1의]" << monster->GetPower() << " 의 데미지!" << endl;
	//			player->GetDameged(monster->GetPower());
	//		}
	//	}
	//}
#pragma endregion

#pragma region Monster2 Attacks
	//if (Monster2Attacks()) // 몬스터 2번타입이 공격상태일때
	//{
	//	if (player->GetHitCol()->colOnOff && // 플레이어의 피격 콜라이더가 켜져있고
	//		player->root->anim->GetPlayNum() != 27) // 구르기상태가 아닐때
	//	{
	//		// 몬스터의 공격 콜라이더들이 플레이어의 히트콜라이더에 충돌했을때
	//		// 몬스터2번 타입의 콜라이더는 스킬1, 스킬2 개이다!
	//		if (monster2->GetAktCol1()->Intersect(player->GetHitCol()) ||
	//			monster2->GetAktCol2()->Intersect(player->GetHitCol()))
	//		{
	//			cout << "플레이어 에게 [몬스터 2의] 스킬 " << monster2->GetPower() << " 의 데미지!" << endl;
	//			player->GetDameged(monster2->GetPower());
	//		}
	//	}
	//}
#pragma endregion
}

void BossScene::PreRender() // 1차 렌더 (특수과 적용을 위한 렌더 부분)
{
	// RTT 렌더링을 통한 특수효과는 적용시킬지 차후 확인 필요(현재로는 특수효과예정X)
	
	// 그림자효과 적용 먼저 렌더링
	ShadowShaderSet();
}

void BossScene::Render() // 2차 렌더 (PreRender이후 진행할 렌더)
{
	LIGHT->Set();// 조명 세팅
	Camera::main->Set(); // 카메라 세팅
	shadow->SetRGBTexture(7); // 그림자 텍스처 적용

	// 멤버들 렌더
	sky->Render();
	map->Render();
	boss->Render();
	player->Render();
	monster->Render();
	monster2->Render();

	//for (int i = 0; i < 3; i++)
	//	map_stone[i]->Render();

	// 보스 상태에 따로 필요한 UI 렌더 조건
	if (boss->bState != MonsterBoss::Boss_State::INTRO)
	{
		gameui->Render();
		//if(isintro == true && boss->GetHP() > 0)
		if (boss->bState != MonsterBoss::Boss_State::DEATH) // 보스상태조건으로 변경했습니다.
			gameui->GetBossBar()->Render();
	}

	// 화면에서 프레임 확인하기용 (프레임 60이하로 떨어지지 않도록 조절해야합니다)
	DWRITE->RenderText(L"FRAME : " + to_wstring(TIMER->GetFramePerSecond()), 
		RECT{ 5,5,static_cast<LONG>(App.GetWidth()),static_cast<LONG>(App.GetHeight())});
}

void BossScene::ResizeScreen() // 카메라 화면 크기 조절
{
	Camera::main->width = App.GetWidth();
	Camera::main->height = App.GetHeight();
	Camera::main->viewport.width = App.GetWidth();
	Camera::main->viewport.height = App.GetHeight();
}

void BossScene::CutScene()
{
	Vector3 vtemp = player->root->GetLocalPos() - boss->root->GetWorldPos();
	float temp = vtemp.Length();

	// 인트로 이벤트용 조건문
	if (!isintro && introcount > 0)
	{
		if (temp < 200)
		{
			//cout << "보스 인트로 진행 시작!" << endl;
			boss->bState = MonsterBoss::Boss_State::INTRO;
			boss->isintro = true;
			isintro = true;
			CamisPlayer = false;
		}
	}
	if (introcount < 1)
	{
		introcount++;
	}

	// 카메라 액션 관련
	if (boss->bState == MonsterBoss::Boss_State::INTRO)
	{
		// 카메라 시점 변경용 좌표
		if (boss->root->anim->GetPlayTime() < 1.0f)
		{
			Camera::main = (Camera*)boss->root->Find("CameraPos"); // 보스의 카메라 액터 좌표하나만들어두기
		}
		// 보스 인트로씬에서 카메라 액션실행
		camAction();
	}
	camCheck(); // 메인캠이 cam이아니라면 3초동안 동작후 메인캠을 cam으로 전환
}

void BossScene::camAction()
{
	//카메라 와야할 위치 (보스의 카메라포인트와 현재 메인캠위치의 벡터를 구한다)
	//Vector3 dis = cam->GetWorldPos() - Camera::main->GetWorldPos();
	//Vector3 dis = boss->root->GetWorldPos() - cam->GetWorldPos();
	boss->root->Find("CameraPos")->SetWorldPosY(20);
	Vector3 dis = boss->root->Find("CameraPos")->GetWorldPos() - cam->GetWorldPos();

	// 메인캠이 아닐때 즉,보스에 포커스가 맞춰진 캠일때만 무빙효과 적용
	if (Camera::main != cam)
	{
		Camera::main->MoveWorldPos(dis * 0.025f * DELTA);
	}

	//세팅된 카메라 위치에서 보스를 바라보는 뷰
	Camera::main->view = Matrix::CreateLookAt(Camera::main->GetWorldPos(),
		boss->root->GetWorldPos(), Vector3(0, 1, 0));

	//기본 프로젝션
	Camera::main->proj = Matrix::CreatePerspectiveFieldOfView(
		Camera::main->fov, Camera::main->width / Camera::main->height, Camera::main->nearZ, Camera::main->farZ);
	Camera::main->SetShadow();
}

void BossScene::camCheck()
{
	if (Camera::main != cam) // 메인 카메라가 바뀌었을떄
	{
		camtimer += DELTA;
		if (camtimer >= 7) //타이머
		{
			camtimer = 0; // 타이머 초기화
			Camera::main = cam; // 메인카메라 되돌리기
			cam->LoadFile("BossSceneCam.xml");
			iscutscene = true; // 컷씬용 변수 true전환.
			CamisPlayer = true; // 카메라 다시 플레리어로 고정 전환
			//기본 프로젝션 재설정
			ResizeScreen();
			//Camera::main->proj = Matrix::CreatePerspectiveFieldOfView(
			//	Camera::main->fov, Camera::main->width / Camera::main->height, Camera::main->nearZ, Camera::main->farZ);
			//Camera::main->SetShadow();
		}
	}
}

bool BossScene::PlayerAttaks()
{
	if (player->root->anim->GetPlayNum() == 2 ||	// STRONG_ATTACK
		player->root->anim->GetPlayNum() == 7 ||	// NORMAL_ATTACK
		player->root->anim->GetPlayNum() == 20 ||	// BIG_SIDE_ATTACK
		player->root->anim->GetPlayNum() == 28 ||	// SECOND_ATTACK
		player->root->anim->GetPlayNum() == 29 ||	// THIRD_ATTACK
		player->root->anim->GetPlayNum() == 31 ||	// JUMP_ATTACK
		player->root->anim->GetPlayNum() == 32 ||	// SECOND_WEAPON_FIRST_ATTACK
		player->root->anim->GetPlayNum() == 33 ||	// SECOND_WEAPON_SECOND_ATTACK
		player->root->anim->GetPlayNum() == 34 ||	// SECOND_WEAPON_THIRD_ATTACK
		player->root->anim->GetPlayNum() == 35		// SECOND_WEAPON_STRONG_ATTACK
		)
	{
		if (player->root->anim->GetPlayTime() > 0.5 &&
			player->root->anim->GetPlayTime() < 0.7)
			return true;
	}

	else
		return false;
}

bool BossScene::BossAttacks()
{
	if (boss->root->anim->GetPlayNum() > 6 && boss->root->anim->GetPlayNum() < 12)
		return true;
	else
		return false;
}

bool BossScene::BossSkillAttacks()
{
	if (boss->bState == MonsterBoss::Boss_State::SKILL) // 스킬상태라면
		return true;
	else
		return false;
}

bool BossScene::MonsterAttacks()
{
	if (monster->root->anim->GetPlayNum() > 4 && monster->root->anim->GetPlayNum() < 8) // 공격모션일때
		return true;
	else
		return false;
}

bool BossScene::Monster2Attacks()
{
	if (monster2->root->anim->GetPlayNum() > 5 && monster2->root->anim->GetPlayNum() < 8) // 공격모션일때
		return true;
	else
		return false;
}

void BossScene::ShadowShaderSet()
{
	shadow->SetTarget(player->root->GetWorldPos()); // 그림자는 플레이어 기준으로
	player->root->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
	player->axe->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
	player->sword->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
	player->Lefthand->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
	boss->root->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
	boss->GetRocks1()->root->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
	boss->GetRocks2()->root->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
	boss->GetRocks3()->root->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
	monster->root->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
	monster2->root->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
	map->Render(RESOURCE->shaders.Load("5.ShadowMap.hlsl"));
}
