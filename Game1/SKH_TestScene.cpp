#include "stdafx.h"
#include <DirectXMath.h>

// 방향벡터 dir을 rotation 각도로 변경
DirectX::XMVECTOR RotateVectorWithEulerAngles(const DirectX::XMVECTOR& dir, float pitch, float yaw, float roll) {
	// Create a rotation matrix from Euler angles (pitch, yaw, roll)
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Rotate the direction vector
	DirectX::XMVECTOR rotatedDir = DirectX::XMVector3TransformCoord(dir, rotationMatrix);

	return rotatedDir;
}

SKH_TestScene::SKH_TestScene()
{
}

SKH_TestScene::~SKH_TestScene()
{
}

void SKH_TestScene::Init()
{
	cam = Camera::Create();
	cam->LoadFile("PPTCAMPOS.xml");
	Camera::main = cam;

	cam2 = Camera::Create();

	sky = Sky::Create();
	sky->texCube->LoadFile("sunsetcube1024.dds"); // 멀리 산능선이 보이는 해직녘풍경 큐브맵

	map = Terrain::Create();
	//map->LoadFile("Terrain.xml");
	map->LoadFile("DJ_Terrain.xml");
	map->showNode = true;
	map->UpdateMeshUv();
	map->CreateStructuredBuffer();
	map->material->shadow = 0.5f; // 그림자가 표현될 수치 적용시키기 (변동없을예정)
	map->showNode = false;

	//임시 플레이어 - 현재 단순한 움직임만 가능하며 카메라모션은 모두 주석처리상태
	player = new APlayer();
	player->Init();
	player->root->name = "Player";
	//player2 = Actor::Create("Player_Sample");
	//player2->LoadFile("TestScenePlayer.xml");

	// 보스
	boss = new MonsterBoss();
	boss->Init();
	boss->root->name = "BOSS";
	boss->root->SetWorldPos(Vector3(0, 0, -250));
	boss->GetBoard()->SetObj(player->root); // 트리 동작용 세팅
	boss->GetBoard()->SetTerrain(map); // 트리 동작용 세팅
	boss->bState = MonsterBoss::Boss_State::NONE;
	boss->root->anim->ChangeAnimation(AnimationState::LOOP, 1);

	//player->SetTarget(boss->root);

	// 테스트용 근접공격형 몬스터
	monster = new MonsterType1();
	monster->Init();
	monster->root->name = "MONSTER_TYPE1";
	monster->SetPos(Vector3(50, 0, -100));
	monster->GetBoard()->SetObj(player->root); // 트리 동작용 세팅
	monster->GetBoard()->SetTerrain(map);
	monster->mState = MonsterType1::Monster_State::NONE;
	monster->root->anim->ChangeAnimation(AnimationState::LOOP, 1);

	// 테스트용 원거리 공격형 몬스터
	monster2 = new MonsterType2();
	monster2->Init();
	monster2->root->name = "MONSTER_TYPE2";
	monster2->SetPos(Vector3(-50, 0, -100));
	monster2->GetBoard()->SetObj(player->root); // 트리 동작용 세팅
	monster2->GetBoard()->SetTerrain(map);
	monster2->mState = MonsterType2::Monster_State::NONE;
	monster2->root->anim->ChangeAnimation(AnimationState::LOOP, 0);





	// 그림자 2,4 그림자맵핑용 쉐이더파일 로드
	shadow = new Shadow();
	RESOURCE->shaders.Load("4.ShadowMap.hlsl"); // Actor 클래스에 적용가능
	RESOURCE->shaders.Load("2.ShadowMap.hlsl"); // Terrain 클래스에 적용가능

	// 출력 화면 크기 조절용
	ResizeScreen();
	
	Camera::mainCamSpeed = 30.0f; // 자유시점 이동 속도 지정 ( 런타임중에는 Gui의 메뉴를 이용해주세요 )
}

void SKH_TestScene::Release()
{
	// 해제 순서는 할당순서의 반대
	SafeDelete(shadow);

	// 테스트용 몬스터
	SafeDelete(monster2);
	SafeDelete(monster);

	SafeDelete(boss);
	SafeDelete(player);

	//player2->Release();
	map->Release();
	sky->Release();
	cam->Release();
}

void SKH_TestScene::Update()
{	
	// 카메라 움직임
	Camera::ControlMainCam(); // 카메라만 따로 움직이고싶다면 주석해제
	ImGui::SliderFloat("CAMSPEED", &Camera::mainCamSpeed, 0.0f, 100.0f);
	LIGHT->RenderDetail();

#pragma region Dir_TO_Rotation_Test
	//// Your direction vector
	////DirectX::XMVECTOR dir = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	//DirectX::XMVECTOR dir = DirectX::XMVectorSet(LIGHT->dirLight.direction.x,
	//											 LIGHT->dirLight.direction.y, 
	//											 LIGHT->dirLight.direction.z, 
	//											 LIGHT->dirLight.direction.w);

	//// Rotation angles in radians
	//float pitch = DirectX::XMConvertToRadians(180.0f); // X-axis rotation
	//float yaw = DirectX::XMConvertToRadians(180.0f);   // Y-axis rotation
	//float roll = DirectX::XMConvertToRadians(180.0f);  // Z-axis rotation

	//// Rotate the direction vector
	//DirectX::XMVECTOR rotatedDir = RotateVectorWithEulerAngles(dir, pitch, yaw, roll);
	//cam2->rotation = rotatedDir;
	//// The rotatedDir vector now contains the direction vector after rotation
#pragma endregion

	if (INPUT->KeyDown(VK_NUMPAD7))
	{
		Camera::main = cam;
	}
	if (INPUT->KeyDown(VK_NUMPAD8))
	{
		Camera::main = cam2;
	}

	// 하이어라이키
	if (ImGui::Begin("Hierarchy"))
	{
		Camera::main->RenderHierarchy();
		sky->RenderHierarchy();
		boss->RenderHierarchy();
		//player2->RenderHierarchy();
		monster->RenderHierarchy();
		monster2->RenderHierarchy();
		ImGui::End();
	}
	shadow->RenderDetail();
	player->RenderHierarchy();
	map->RenderHierarchy();

	// 객체들 업데이트
	Camera::main->Update();
	sky->Update();
	map->Update();
	player->Update();
	//player2->Update();
	boss->Update();
	monster->Update();
	monster2->Update();

}

void SKH_TestScene::LateUpdate()
{
	
}

void SKH_TestScene::PreRender()
{
	// 프리렌더에서 그림자먼저 렌더링 한다음 렌더진행
	// 4.ShadowMap.hlsl = Actor 클래스에 적용
	// 5.ShadowMap.hlsl = Terrain 클래스에 적용
	// 현재 2개타입 클래스만 그림자적용합니다. 필요시 복사해서 코드를 수정해주세요! (주의. 타입별 넘버링 겹치지 않도록)

	// 플레이어쪽 그림자들
	shadow->SetTarget(player->root->GetWorldPos()); // 그림자생성의 기준은 플레이어위치
	player->root->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
	player->axe->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
	player->sword->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
	player->Lefthand->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
	// 보스와 스킬이펙트 그림자들
	boss->root->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
	//boss->GetRocks1()->root->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
	//boss->GetRocks2()->root->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
	//boss->GetRocks3()->root->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
	// 몬스터 그림자
	monster->root->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
	monster2->root->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
	// 높낮이있을경우 맵에도 그림자 생성
	map->Render(RESOURCE->shaders.Load("5.ShadowMap.hlsl"));
}

void SKH_TestScene::Render()
{
	LIGHT->Set();// 조명 세팅
	Camera::main->Set(); // 카메라 세팅
	shadow->SetRGBTexture(7); // 그림자 텍스처 적용

	// 멤버들 렌더
	sky->Render();
	map->Render();
	boss->Render();
	player->Render();
	//player2->Render();
	monster->Render();
	monster2->Render();

	// DWrite Rendering
	
	// 몬스터 이펙트 적용 출력 테스트
	//RenderTexture();

	// 큐브맵 변경 테스트
	//ChangeCubemap();

	// 그림자 관련 텍스트 출력
	ShadowDwrite();
}

void SKH_TestScene::ResizeScreen()
{
	Camera::main->width = App.GetWidth();
	Camera::main->height = App.GetHeight();
	Camera::main->viewport.width = App.GetWidth();
	Camera::main->viewport.height = App.GetHeight();
}

void SKH_TestScene::RenderTexture()
{

#pragma region Boss_Sample
	/* 
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
	15. 패리 그로기(그로기상태에서 패리성공 실패 나뉨)
	16. 패리 공격넉백(패리 성공되었을때 1)
	17. 패리 당한뒤 일어나는 모션(패리 성공되었을때 2)
	18. 패리 그로기상태에서 일어나기(패리 실패되었을때)	
	*/

	// 모션 샘플 키입력(보스)
	//if (INPUT->KeyDown(VK_NUMPAD1))
	//{
	//	boss->root->anim->ChangeAnimation(AnimationState::ONCE_FIRST, 7);
	//}
	//if (INPUT->KeyDown(VK_NUMPAD2))
	//{
	//	boss->root->anim->ChangeAnimation(AnimationState::ONCE_FIRST, 9);
	//}
	//if (INPUT->KeyDown(VK_NUMPAD3))
	//{
	//	boss->root->anim->ChangeAnimation(AnimationState::ONCE_FIRST, 11);
	//}
	//if (INPUT->KeyDown(VK_NUMPAD0))
	//{
	//	boss->bState = MonsterBoss::Boss_State::DEATH;
	//}
	//if (INPUT->KeyDown(VK_NUMPAD4))
	//{
	//	boss->bState = MonsterBoss::Boss_State::SKILL;
	//	boss->SetSkills(0);
	//}
	//if (INPUT->KeyDown(VK_NUMPAD5))
	//{
	//	boss->bState = MonsterBoss::Boss_State::SKILL;
	//	boss->SetSkills(1);
	//}

	//wstring temp;
	//switch (boss->root->anim->GetPlayNum())
	//{
	//case 6:
	//	if (boss->GetSkillType() == 0)
	//	{
	//		temp = L"Pase2 Skill - Pop Effect";
	//	}
	//	else
	//	{
	//		temp = L"Pase1 Skill - Pop Effect";
	//	}
	//	break;
	//case 7:
	//	temp = L"Right hand Attack - Slash Trail Effect";
	//	break;
	//case 9:
	//	temp = L"Left hand Attack - Slash Trail Effect";
	//	break;
	//case 11:
	//	temp = L"Heavy Slash Attack - Slash Trail Effect";
	//	break;
	//case 12:
	//	temp = L"Death Effect - Pop Effect";
	//	break;
	//case 13:
	//	temp = L"Death Effect - Rain Effect";
	//	break;
	//default:
	//	temp = L" ";
	//	break;
	//}
#pragma endregion

#pragma region Monster1_Sample
	/*	
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
	// 모션 샘플 키입력 (몬스터1타입)
	//if (INPUT->KeyDown(VK_NUMPAD1))
	//{
	//	//monster->mState = MonsterType1::Monster_State::HIT;
	//	monster->root->anim->ChangeAnimation(AnimationState::ONCE_FIRST, 3);
	//	monster->GetBloodEffect()->Play();
	//	monster->GetBloodEffect()->SetPos(monster->root->GetWorldPos());
	//	int temp = RANDOM->Int(7, 14);
	//	monster->GetBloodEffect()->SetPosY(temp);
	//}
	//if (INPUT->KeyDown(VK_NUMPAD2))
	//{
	//	monster->root->anim->ChangeAnimation(AnimationState::ONCE_FIRST, 5);
	//}
	//if (INPUT->KeyDown(VK_NUMPAD3))
	//{
	//	monster->root->anim->ChangeAnimation(AnimationState::ONCE_FIRST, 6);
	//}
	//if (INPUT->KeyDown(VK_NUMPAD4))
	//{
	//	monster->root->anim->ChangeAnimation(AnimationState::ONCE_FIRST, 7);
	//}
	//if (INPUT->KeyDown(VK_NUMPAD0))
	//{
	//	monster->mState = MonsterType1::Monster_State::DEATH;
	//}
	
	//wstring temp;
	//switch (monster->root->anim->GetPlayNum())
	//{
	//case 3:
	//	temp = L"Monster1 Hit - Pop Effect";
	//	break;
	//case 5:
	//	temp = L"Double Hand Attack - Pop Effect";
	//	break;
	//case 6:
	//	temp = L"One Hand Attack - Slash Trail Effect";
	//	break;
	//case 7:
	//	temp = L"Foot Attack - Pop Effect";
	//	break;
	//case 4:
	//	temp = L"Death Effect - Rain Effect";
	//	break;
	//default:
	//	temp = L" ";
	//	break;
	//}
#pragma endregion


#pragma region Monster2_Sample
	/*
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
	
	// 모션 샘플 키입력 (몬스터2타입)
	if (INPUT->KeyDown(VK_NUMPAD1))
	{
		//monster->mState = MonsterType1::Monster_State::HIT;
		monster2->root->anim->ChangeAnimation(AnimationState::ONCE_FIRST, 2);
		monster2->GetBloodEffect()->Play();
		monster2->GetBloodEffect()->SetPos(monster2->root->GetWorldPos());
		int temp = RANDOM->Int(7, 14);
		monster2->GetBloodEffect()->SetPosY(temp);
	}
	if (INPUT->KeyDown(VK_NUMPAD2))
	{
		monster2->root->anim->ChangeAnimation(AnimationState::ONCE_FIRST, 6);
	}
	if (INPUT->KeyDown(VK_NUMPAD3))
	{
		monster2->root->anim->ChangeAnimation(AnimationState::ONCE_FIRST, 7);
	}
	if (INPUT->KeyDown(VK_NUMPAD0))
	{
		monster2->mState = MonsterType2::Monster_State::DEATH;
	}


	wstring temp;
	switch (monster2->root->anim->GetPlayNum())
	{
	case 2:
		temp = L"Monster1 Hit - Pop Effect";
		break;
	case 6:
		temp = L"Fire Wall Attack - Slash Trail Effect";
		break;
	case 7:
		temp = L"Icicle Rain Attack - Pop Effect";
		break;
	case 3:
		temp = L"Death Effect - Rain Effect";
		break;
	case 4:
		temp = L"Death Effect - Rain Effect";
		break;
	default:
		temp = L" ";
		break;
	}
#pragma endregion

	DWRITE->RenderText(temp,RECT{ 200,125,static_cast<LONG>(App.GetWidth()),static_cast<LONG>(App.GetHeight()) },25.0f);
}

void SKH_TestScene::ChangeCubemap()
{
	if (INPUT->KeyDown(VK_NUMPAD0))
	{
		mapchange = !mapchange;
	}
	if (mapchange)
	{
		cam->rotation.y += Camera::mainCamSpeed * TORADIAN * DELTA;

		if (TIMER->GetTick(zerotimer, 2.5f))
		{
			count++;
			if (count > 3)
			{
				count = 0;
			}
		}

		switch (count)
		{
		case 0:
		{
			sky->texCube->LoadFile("sunsetcube1024.dds"); // 멀리 산능선이 보이는 해직녘풍경 큐브맵
			backgroundname = L"Cube Map Style : SunSet";
		}
		break;
		case 1:
		{
			sky->texCube->LoadFile("desertcube1024.dds"); // 멀리 산능선이 보이는 해직녘풍경 큐브맵
			backgroundname = L"Cube Map Style : Desert";
		}
		break;
		case 2:
		{
			sky->texCube->LoadFile("grasscube1024.dds"); // 멀리 산능선이 보이는 해직녘풍경 큐브맵
			backgroundname = L"Cube Map Style : Grass";
		}
		break;
		case 3:
		{
			sky->texCube->LoadFile("snowcube1024.dds"); // 멀리 산능선이 보이는 해직녘풍경 큐브맵
			backgroundname = L"Cube Map Style : Snow";
		}
		break;
		}
	}
	



	DWRITE->RenderText(backgroundname, RECT{ 200,125,static_cast<LONG>(App.GetWidth()),static_cast<LONG>(App.GetHeight()) }, 25.0f
	,L"던파 연단된 칼날");

}

void SKH_TestScene::ShadowDwrite() // 그림자정보 관련 출력 함수
{

	DWRITE->RenderText(L"그림자 출력 범위 : "+to_wstring((int)shadow->range), 
		RECT{200,125,static_cast<LONG>(App.GetWidth()),static_cast<LONG>(App.GetHeight())}, 25.0f
		, L"던파 연단된 칼날");

	DWRITE->RenderText(L"그림자 Texture 크기" + to_wstring((int)shadow->GetTextureSize()), 
		RECT{ 200,155,static_cast<LONG>(App.GetWidth()),static_cast<LONG>(App.GetHeight()) }, 25.0f
		, L"던파 연단된 칼날");
	switch (shadow->GetQuality())
	{
	case 0:
		DWRITE->RenderText(L"Texture 필터 : MIN_MAG_MIP_POINT", 
			RECT{ 200,185,static_cast<LONG>(App.GetWidth()),static_cast<LONG>(App.GetHeight()) }, 25.0f
			, L"던파 연단된 칼날");
		break;
	case 1:
		DWRITE->RenderText(L"Texture 필터 : MIN_MAG_MIP_LINEAR", 
			RECT{ 200,185,static_cast<LONG>(App.GetWidth()),static_cast<LONG>(App.GetHeight()) }, 25.0f
			, L"던파 연단된 칼날");
		break;
	}
}
