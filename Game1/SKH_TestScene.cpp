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
	sky->texCube->LoadFile("sunsetcube1024.dds"); // �ָ� ��ɼ��� ���̴� ������ǳ�� ť���

	map = Terrain::Create();
	//map->LoadFile("Terrain.xml");
	map->LoadFile("DJ_Terrain.xml");
	map->showNode = true;
	map->UpdateMeshUv();
	map->CreateStructuredBuffer();
	map->material->shadow = 0.5f; // �׸��ڰ� ǥ���� ��ġ �����Ű�� (������������)
	map->showNode = false;

	//�ӽ� �÷��̾� - ���� �ܼ��� �����Ӹ� �����ϸ� ī�޶����� ��� �ּ�ó������
	player = new APlayer();
	player->Init();

	// ����
	boss = new MonsterBoss();
	boss->Init();
	boss->root->SetWorldPos(Vector3(0, 0, -250));
	boss->GetBoard()->SetObj(player->root); // Ʈ�� ���ۿ� ����
	boss->GetBoard()->SetTerrain(map); // Ʈ�� ���ۿ� ����
	boss->bState = MonsterBoss::Boss_State::NONE;
	boss->root->anim->ChangeAnimation(AnimationState::LOOP, 1);

	//player->SetTarget(boss->root);

	// �׽�Ʈ�� ���������� ����
	monster = new MonsterType1();
	monster->Init();
	monster->SetPos(Vector3(50, 0, -100));
	monster->GetBoard()->SetObj(player->root); // Ʈ�� ���ۿ� ����
	monster->GetBoard()->SetTerrain(map);
	monster->mState = MonsterType1::Monster_State::NONE;
	monster->root->anim->ChangeAnimation(AnimationState::LOOP, 1);

	// �׽�Ʈ�� ���Ÿ� ������ ����
	monster2 = new MonsterType2();
	monster2->Init();
	monster2->SetPos(Vector3(-50, 0, -100));
	monster2->GetBoard()->SetObj(player->root); // Ʈ�� ���ۿ� ����
	monster2->GetBoard()->SetTerrain(map);
	monster2->mState = MonsterType2::Monster_State::NONE;
	monster2->root->anim->ChangeAnimation(AnimationState::LOOP, 0);



	// �׸��� 2,4 �׸��ڸ��ο� ���̴����� �ε�
	shadow = new Shadow();
	RESOURCE->shaders.Load("4.ShadowMap.hlsl"); // Actor Ŭ������ ���밡��
	RESOURCE->shaders.Load("2.ShadowMap.hlsl"); // Terrain Ŭ������ ���밡��

	// ��� ȭ�� ũ�� ������
	ResizeScreen();
	
	Camera::mainCamSpeed = 100.0f; // �������� �̵� �ӵ� ���� ( ��Ÿ���߿��� Gui�� �޴��� �̿����ּ��� )
}

void SKH_TestScene::Release()
{
	// ���� ������ �Ҵ������ �ݴ�
	SafeDelete(shadow);

	// �׽�Ʈ�� ����
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
	// ī�޶� ������
	Camera::ControlMainCam(); // ī�޶� ���� �����̰�ʹٸ� �ּ�����
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








	// ���̾����Ű
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

	// ��ü�� ������Ʈ
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
	// ������������ �׸��ڸ��� ������ �Ѵ��� ��������
		// 4.ShadowMap.hlsl = Actor Ŭ������ ����
	// 5.ShadowMap.hlsl = Terrain Ŭ������ ����
	// ���� 2��Ÿ�� Ŭ������ �׸��������մϴ�. �ʿ�� �����ؼ� �ڵ带 �������ּ���! (����. Ÿ�Ժ� �ѹ��� ��ġ�� �ʵ���)

	shadow->SetTarget(player->root->GetWorldPos()); // �׸��ڻ����� ������ �÷��̾���ġ
	// �÷��̾��� �׸��ڵ�
	player->root->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
	player->axe->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
	player->sword->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
	player->Lefthand->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
	// ������ ��ų����Ʈ �׸��ڵ�
	boss->root->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
	boss->GetRocks1()->root->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
	boss->GetRocks2()->root->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
	boss->GetRocks3()->root->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
	// ���� �׸���
	monster->root->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
	monster2->root->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
	// ������������� �ʿ��� �׸��� ����
	map->Render(RESOURCE->shaders.Load("5.ShadowMap.hlsl"));
}

void SKH_TestScene::Render()
{
	LIGHT->Set();// ���� ����
	Camera::main->Set(); // ī�޶� ����
	shadow->SetRGBTexture(7); // �׸��� �ؽ�ó ����

	// ����� ����
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
	15. �и� �׷α�(�׷α���¿��� �и����� ���� ����)
	16. �и� ���ݳ˹�(�и� �����Ǿ����� 1)
	17. �и� ���ѵ� �Ͼ�� ���(�и� �����Ǿ����� 2)
	18. �и� �׷α���¿��� �Ͼ��(�и� ���еǾ�����)	
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
