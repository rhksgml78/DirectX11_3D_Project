#include "stdafx.h"

SKH_TestScene::SKH_TestScene()
{
}

SKH_TestScene::~SKH_TestScene()
{
}

void SKH_TestScene::Init()
{

	//grid = Grid::Create();

	map = Terrain::Create();
	map->LoadFile("SKH_Stone_Tile.xml");
	map->scale.x = 10;
	map->scale.z = 10;
	map->uvScale = 100;
	map->UpdateMeshUv();
	map->CreateStructuredBuffer();
	//그림자가 표현될 수치 적용시키기
	map->material->shadow = 0.5f;

	cam = Camera::Create();
	cam->LoadFile("SKHTESTCAM.xml");
	Camera::main = cam;



	player = Actor::Create();
	player->LoadFile("Monster1_Temp.xml");
	player->anim->ChangeAnimation(AnimationState::LOOP,1);

	float temp = 0.5f;
	player->root->Find("PumpkinHulk")->material->ambient = Color(temp, temp, temp, 1.0f);
	player->root->Find("PumpkinHulk")->material->emissive = Color(temp, temp, temp, 1.0f);

	// 보스
	boss = new MonsterBoss();
	boss->Init();
	//boss->root->SetWorldPos(Vector3(0,0,-250));
	boss->root->SetWorldPos(Vector3(0,0,50));

	boss->GetBoard()->SetObj(player->root);
	//boss->GetBoard()->SetTerrain(map);


	// 몬스터
	for (int i = 0; i < MONSTERNUM; i++)
	{
		float tempx = RANDOM->Float(-50.0f,0.0f);
		float tempz = RANDOM->Float(-100.0f, -50.0f);
		float temprot = RANDOM->Float(0.0f, PI);
		monster[i] = new MonsterType1();
		monster[i]->Init();
		monster[i]->SetPos(Vector3(tempx, 0, tempz));
		monster[i]->root->rotation.y = temprot;
		monster[i]->GetBoard()->SetObj(player->root);
		monster[i]->GetBoard()->SetTerrain(map);
	}

	for (int i = 0; i < MONSTERNUM; i++)
	{
		float tempx = RANDOM->Float(0.0f, 50.0f);
		float tempz = RANDOM->Float(-100.0f, -50.0f);
		float temprot = RANDOM->Float(0, PI);
		monster2[i] = new MonsterType2();
		monster2[i]->Init();
		monster2[i]->SetPos(Vector3(tempx, 0, tempz));
		monster2[i]->root->rotation.y = temprot;
		monster2[i]->GetBoard()->SetObj(player->root);
		monster2[i]->GetBoard()->SetTerrain(map);
	}
	
	// 그림자
	shadow = new Shadow();
	//shadow2 = new Shadow();
	//ps쉐이더가 다른쉐이더
	RESOURCE->shaders.Load("4.ShadowMap.hlsl");
	RESOURCE->shaders.Load("2.ShadowMap.hlsl");



	//gameui = GameUI::Create();

	// 컷씬용
	isintro = false;

	Camera::main->mainCamSpeed = 200.0f;

}

void SKH_TestScene::Release()
{
	//SafeDelete(grid);
	SafeDelete(boss);
	for (int i = 0; i < MONSTERNUM; i++)
	{
		SafeDelete(monster[i]);
		SafeDelete(monster2[i]);
	}

	//gameui->Release();
}

void SKH_TestScene::Update()
{	
#pragma region State_Change_Test
	// 피격 상태로 변경
	if (INPUT->KeyDown(VK_SPACE))
	{
		for (int i = 0; i < MONSTERNUM; i++)
		{
			if (monster[i]->hp > 0)
			{
				monster[i]->mState = MonsterType1::Monster_State::HIT;
			}
			if (monster2[i]->hp > 0)
			{
				monster2[i]->mState = MonsterType2::Monster_State::HIT;
			}
		}
		boss->GetDameged(2);
		cout <<"보스체력 : " << boss->GetHP() << endl;
	}
	////보스&몬스터 상태 사망으로 변경 시키기
	//if (INPUT->KeyDown(VK_NUMPAD0))
	//{
	//	boss->bState = MonsterBoss::Boss_State::DEATH;
	//	for (int i = 0; i < MONSTERNUM; i++)
	//	{
	//		monster[i]->mState = MonsterType1::Monster_State::DEATH;
	//		monster2[i]->mState = MonsterType2::Monster_State::DEATH;
	//	}
	//}
#pragma endregion

#pragma region Intro_Action_Test
	//// 카메라 액션 관련
	//if (boss->bState == MonsterBoss::Boss_State::INTRO || boss->bState == MonsterBoss::Boss_State::DEATH)
	//{
	//	// 카메라 시점 변경용 좌표
	//	if (boss->root->anim->GetPlayTime() < 1.0f)
	//	{
	//		Camera::main = (Camera*)boss->root->Find("CameraPos"); // 보스의 카메라 액터 좌표하나만들어두기
	//	}
	//	// 보스 인트로씬에서 카메라 액션실행
	//	camAction();
	//}
	//camCheck();

	//Vector3 vtemp = player->GetLocalPos() - boss->root->GetWorldPos();
	//float temp = vtemp.Length();
	//
	//// 인트로 이벤트용 조건문
	//if (!isintro && introcount > 0)
	//{
	//	if (temp < 200)
	//	{
	//		cout << "보스 인트로 진행 시작!" << endl;
	//		boss->bState = MonsterBoss::Boss_State::INTRO;
	//		boss->isintro = true;
	//		isintro = true;
	//	}
	//}
	//if (introcount < 1)
	//{
	//	introcount++;
	//}
#pragma endregion

	move();

	Camera::main->ControlMainCam();

	if (ImGui::Begin("Hierarchy"))
	{
		Camera::main->RenderHierarchy();
		//grid->RenderHierarchy();
		map->RenderHierarchy();
		// 몬스터 관련
		boss->RenderHierarchy();
		for (int i = 0; i < MONSTERNUM; i++)
		{
			monster[i]->RenderHierarchy();
			monster2[i]->RenderHierarchy();
		}

		player->RenderHierarchy();
		//gameui->RenderHierarchy();

		ImGui::End();
	}

	Camera::main->Update();
	map->Update();
	player->Update();
	boss->Update();
	for (int i = 0; i < MONSTERNUM; i++)
	{
		monster[i]->Update();
		monster2[i]->Update();
	}

	//for (int i = 0; i < MONSTERNUM; i++)
	//{
	//	gameui->Update(boss);
	//}

	

}

void SKH_TestScene::LateUpdate()
{
	// 플레이어가 항상 보스를 바라보고 있도록 설정
	// 사용시 주의사항 : 먼저 대상의 좌표(Vector3)가 존재해야하므로
	// 객체들이 업데이트 된 후에 사용하도록 합니다.
	//lookAtrotation();
	if (!player->root->collider->colOnOff)
	{
		if (TIMER->GetTick(temp, 1.0f))
		{
			cout << "플레이어의 콜라이더 ON" << endl;

			player->root->collider->colOnOff = true;
		}
	}

	if (boss->GetHitCol() && player->root->collider->Intersect(boss->GetHitCol()))
	{
		cout << "보스 데미지 3" << endl;
		boss->GetDameged(1);
		player->root->collider->colOnOff = false;
	}

}

void SKH_TestScene::PreRender()
{
	//shadow2->SetTarget(boss->GetPos());
	shadow->SetTarget(player->GetWorldPos());
	player->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
	boss->root->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
	map->Render(RESOURCE->shaders.Load("5.ShadowMap.hlsl"));
	for (int i = 0; i < MONSTERNUM; i++)
	{
		monster[i]->root->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
		monster2[i]->root->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
	}
}

void SKH_TestScene::Render()
{
	// 조명 세팅
	LIGHT->Set();
	Camera::main->Set();
	shadow->SetRGBTexture(7);
	//shadow2->SetRGBTexture(7);

	map->Render();
	boss->Render();
	for (int i = 0; i < MONSTERNUM; i++)
	{
		monster[i]->Render();
		monster2[i]->Render();
	}
	player->Render();

	DWRITE->RenderText(L"FPS : " + to_wstring(TIMER->GetFramePerSecond()), RECT{5,5,500,500}, 40.0f);

	//gameui->Render();
}

void SKH_TestScene::ResizeScreen()
{
	Camera::main->width = App.GetWidth();
	Camera::main->height = App.GetHeight();
	Camera::main->viewport.width = App.GetWidth();
	Camera::main->viewport.height = App.GetHeight();
}

void SKH_TestScene::move()
{
	if (INPUT->KeyPress(VK_LEFT))
	{
		//player->MoveWorldPos(-player->GetRight() * 50 * DELTA);
		player->MoveWorldPos(Vector3(1,0,0) * 50 * DELTA);
	}
	if (INPUT->KeyPress(VK_RIGHT))
	{
		//player->MoveWorldPos(player->GetRight() * 50 * DELTA);
		player->MoveWorldPos(Vector3(-1, 0, 0) * 50 * DELTA);
	}
	if (INPUT->KeyPress(VK_UP))
	{
		//player->MoveWorldPos(player->GetForward() * 50 * DELTA);
		player->MoveWorldPos(Vector3(0, 0, -1) * 50 * DELTA);
	}
	if (INPUT->KeyPress(VK_DOWN))
	{
		//player->MoveWorldPos(-player->GetForward() * 50 * DELTA);
		player->MoveWorldPos(Vector3(0, 0, 1) * 50 * DELTA);
	}
}

void SKH_TestScene::camAction()
{
	//카메라 와야할 위치 (보스의 카메라포인트와 현재 메인캠위치의 벡터를 구한다)
	//Vector3 dis = cam->GetWorldPos() - Camera::main->GetWorldPos();
	Vector3 dis = boss->root->Find("CameraPos")->GetWorldPos() - cam->GetWorldPos();

	// 메인캠이 아닐때 즉,보스에 포커스가 맞춰진 캠일때만 무빙효과 적용
	if (Camera::main != cam)
	{
		Camera::main->MoveWorldPos(dis * 0.01f * DELTA); //(델타를 사용하지 않는이유는 스칼라값에 이미 적용되어있기때문)
	}

	//세팅된 카메라 위치에서 보스를 바라보는 뷰
	Camera::main->view = Matrix::CreateLookAt(Camera::main->GetWorldPos(),
	    boss->root->GetWorldPos(), Vector3(0, 1, 0));
	 
	//기본 프로젝션
	Camera::main->proj  = Matrix::CreatePerspectiveFieldOfView(
	Camera::main->fov, Camera::main->width / Camera::main->height, Camera::main->nearZ, Camera::main->farZ);
	Camera::main->SetShadow();
}

void SKH_TestScene::lookAtrotation()
{
	// 전치행렬관계로 만들어져있는 뷰행렬
	Matrix R = Matrix::CreateLookAt(player->GetWorldPos(), boss->root->GetWorldPos(), Vector3(0, 1, 0));

	// 뷰행렬을 전치행렬로 변환한다.
	R = R.Transpose();

	// 행렬[matrix]에서 -> 쿼터니언[Quaternion]으로 변환
	Quaternion q = Quaternion::CreateFromRotationMatrix(R);

	// 쿼터니언[Quaternion]에서 -> 회전[Rotation]으로 적용
	player->rotation.y = Util::QuaternionToYawPtichRoll(q).y;
}

void SKH_TestScene::camCheck()
{
	if (Camera::main != cam) // 메인 카메라가 바뀌었을떄
	{
		camtimer += DELTA;
		if (camtimer >= 3) // 3초 타이머
		{
			camtimer = 0; // 타이머 초기화
			Camera::main = cam; // 메인카메라 되돌리기
		}
	}
}
