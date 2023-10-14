#include "stdafx.h"

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

	// 보스
	boss = new MonsterBoss();
	boss->Init();
	boss->root->SetWorldPos(Vector3(0, 0, -250));
	boss->GetBoard()->SetObj(player->root); // 트리 동작용 세팅
	boss->GetBoard()->SetTerrain(map); // 트리 동작용 세팅
	boss->bState = MonsterBoss::Boss_State::NONE;
	boss->root->anim->ChangeAnimation(AnimationState::LOOP, 1);

	//player->SetTarget(boss->root);

	// 테스트용 근접공격형 몬스터
	monster = new MonsterType1();
	monster->Init();
	monster->SetPos(Vector3(50, 0, -100));
	monster->GetBoard()->SetObj(player->root); // 트리 동작용 세팅
	monster->GetBoard()->SetTerrain(map);
	monster->mState = MonsterType1::Monster_State::NONE;
	monster->root->anim->ChangeAnimation(AnimationState::LOOP, 1);

	// 테스트용 원거리 공격형 몬스터
	monster2 = new MonsterType2();
	monster2->Init();
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
	
	Camera::mainCamSpeed = 100.0f; // 자유시점 이동 속도 지정 ( 런타임중에는 Gui의 메뉴를 이용해주세요 )
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

	map->Release();
	sky->Release();
	cam->Release();
}

void SKH_TestScene::Update()
{	
	// 카메라 움직임
	Camera::ControlMainCam(); // 카메라만 따로 움직이고싶다면 주석해제
	ImGui::SliderFloat("CAMSPEED", &Camera::mainCamSpeed, 0.0f, 100.0f);



	if (INPUT->KeyDown(VK_NUMPAD1))
	{
		boss->root->anim->ChangeAnimation(AnimationState::ONCE_FIRST, 7);
	}
	if (INPUT->KeyDown(VK_NUMPAD2))
	{
		boss->root->anim->ChangeAnimation(AnimationState::ONCE_FIRST, 9);
	}
	if (INPUT->KeyDown(VK_NUMPAD3))
	{
		boss->root->anim->ChangeAnimation(AnimationState::ONCE_FIRST, 11);
	}
	if (INPUT->KeyDown(VK_NUMPAD0))
	{
		boss->bState = MonsterBoss::Boss_State::DEATH;
	}
	if (INPUT->KeyDown(VK_NUMPAD4))
	{
		boss->bState = MonsterBoss::Boss_State::SKILL;
		boss->SetSkills(0);
	}
	if (INPUT->KeyDown(VK_NUMPAD5))
	{
		boss->bState = MonsterBoss::Boss_State::SKILL;
		boss->SetSkills(1);
	}








	// 하이어라이키
	shadow->RenderDetail();
	if (ImGui::Begin("Hierarchy"))
	{
		Camera::main->RenderHierarchy();
		sky->RenderHierarchy();
		map->RenderHierarchy();
		boss->RenderHierarchy();
		player->RenderHierarchy();
		monster->RenderHierarchy();
		monster2->RenderHierarchy();
		ImGui::End();
	}

	// 객체들 업데이트
	Camera::main->Update();
	sky->Update();
	map->Update();
	player->Update();
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

	shadow->SetTarget(player->root->GetWorldPos()); // 그림자생성의 기준은 플레이어위치
	// 플레이어쪽 그림자들
	player->root->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
	player->axe->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
	player->sword->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
	player->Lefthand->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
	// 보스와 스킬이펙트 그림자들
	boss->root->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
	boss->GetRocks1()->root->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
	boss->GetRocks2()->root->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
	boss->GetRocks3()->root->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
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
	monster->Render();
	monster2->Render();

	// DWrite Rendering
	RenderTexture();
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

	
	wstring temp;
	switch (boss->root->anim->GetPlayNum())
	{
	case 6:
		temp = L"Skill Active - Pop Effect";
		break;
	case 7:
		temp = L"Right hand Attack - Slash Trail Effect";
		break;
	case 9:
		temp = L"Left hand Attack - Slash Trail Effect";
		break;
	case 11:
		temp = L"Heavy Slash Attack - Slash Trail Effect";
		break;
	case 12:
		temp = L"Death Effect - Pop Effect";
		break;
	case 13:
		temp = L"Death Effect - Rain Effect";
		break;
	default:
		temp = L" ";
		break;
	}
	DWRITE->RenderText(temp,RECT{ 200,125,static_cast<LONG>(App.GetWidth()),static_cast<LONG>(App.GetHeight()) },25.0f);
}
