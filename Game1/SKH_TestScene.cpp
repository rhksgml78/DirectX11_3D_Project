#include "stdafx.h"
#include <DirectXMath.h>

// ���⺤�� dir�� rotation ������ ����
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
	player->root->name = "Player";
	//player2 = Actor::Create("Player_Sample");
	//player2->LoadFile("TestScenePlayer.xml");

	// ����
	boss = new MonsterBoss();
	boss->Init();
	boss->root->name = "BOSS";
	boss->root->SetWorldPos(Vector3(0, 0, -250));
	boss->GetBoard()->SetObj(player->root); // Ʈ�� ���ۿ� ����
	boss->GetBoard()->SetTerrain(map); // Ʈ�� ���ۿ� ����
	boss->bState = MonsterBoss::Boss_State::NONE;
	boss->root->anim->ChangeAnimation(AnimationState::LOOP, 1);

	//player->SetTarget(boss->root);

	// �׽�Ʈ�� ���������� ����
	monster = new MonsterType1();
	monster->Init();
	monster->root->name = "MONSTER_TYPE1";
	monster->SetPos(Vector3(50, 0, -100));
	monster->GetBoard()->SetObj(player->root); // Ʈ�� ���ۿ� ����
	monster->GetBoard()->SetTerrain(map);
	monster->mState = MonsterType1::Monster_State::NONE;
	monster->root->anim->ChangeAnimation(AnimationState::LOOP, 1);

	// �׽�Ʈ�� ���Ÿ� ������ ����
	monster2 = new MonsterType2();
	monster2->Init();
	monster2->root->name = "MONSTER_TYPE2";
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
	
	Camera::mainCamSpeed = 30.0f; // �������� �̵� �ӵ� ���� ( ��Ÿ���߿��� Gui�� �޴��� �̿����ּ��� )
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

	//player2->Release();
	map->Release();
	sky->Release();
	cam->Release();
}

void SKH_TestScene::Update()
{	
	// ī�޶� ������
	Camera::ControlMainCam(); // ī�޶� ���� �����̰�ʹٸ� �ּ�����
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

	// ���̾����Ű
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

	// ��ü�� ������Ʈ
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
	// ������������ �׸��ڸ��� ������ �Ѵ��� ��������
	// 4.ShadowMap.hlsl = Actor Ŭ������ ����
	// 5.ShadowMap.hlsl = Terrain Ŭ������ ����
	// ���� 2��Ÿ�� Ŭ������ �׸��������մϴ�. �ʿ�� �����ؼ� �ڵ带 �������ּ���! (����. Ÿ�Ժ� �ѹ��� ��ġ�� �ʵ���)

	// �÷��̾��� �׸��ڵ�
	shadow->SetTarget(player->root->GetWorldPos()); // �׸��ڻ����� ������ �÷��̾���ġ
	player->root->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
	player->axe->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
	player->sword->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
	player->Lefthand->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
	// ������ ��ų����Ʈ �׸��ڵ�
	boss->root->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
	//boss->GetRocks1()->root->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
	//boss->GetRocks2()->root->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
	//boss->GetRocks3()->root->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
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
	//player2->Render();
	monster->Render();
	monster2->Render();

	// DWrite Rendering
	
	// ���� ����Ʈ ���� ��� �׽�Ʈ
	//RenderTexture();

	// ť��� ���� �׽�Ʈ
	//ChangeCubemap();

	// �׸��� ���� �ؽ�Ʈ ���
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

	// ��� ���� Ű�Է�(����)
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
	Monster1 �ִϸ��̼� ����Ʈ
	0. ���ٽ� ��ȿ
	1. �⺻ idle
	2. �̵� move
	3. �ǰ� hit
	4. ��� death
	5. ���� ���������
	6. ���� �Ѽ�
	7. ���� ������
	*/
	// ��� ���� Ű�Է� (����1Ÿ��)
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
	
	// ��� ���� Ű�Է� (����2Ÿ��)
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
			sky->texCube->LoadFile("sunsetcube1024.dds"); // �ָ� ��ɼ��� ���̴� ������ǳ�� ť���
			backgroundname = L"Cube Map Style : SunSet";
		}
		break;
		case 1:
		{
			sky->texCube->LoadFile("desertcube1024.dds"); // �ָ� ��ɼ��� ���̴� ������ǳ�� ť���
			backgroundname = L"Cube Map Style : Desert";
		}
		break;
		case 2:
		{
			sky->texCube->LoadFile("grasscube1024.dds"); // �ָ� ��ɼ��� ���̴� ������ǳ�� ť���
			backgroundname = L"Cube Map Style : Grass";
		}
		break;
		case 3:
		{
			sky->texCube->LoadFile("snowcube1024.dds"); // �ָ� ��ɼ��� ���̴� ������ǳ�� ť���
			backgroundname = L"Cube Map Style : Snow";
		}
		break;
		}
	}
	



	DWRITE->RenderText(backgroundname, RECT{ 200,125,static_cast<LONG>(App.GetWidth()),static_cast<LONG>(App.GetHeight()) }, 25.0f
	,L"���� ���ܵ� Į��");

}

void SKH_TestScene::ShadowDwrite() // �׸������� ���� ��� �Լ�
{

	DWRITE->RenderText(L"�׸��� ��� ���� : "+to_wstring((int)shadow->range), 
		RECT{200,125,static_cast<LONG>(App.GetWidth()),static_cast<LONG>(App.GetHeight())}, 25.0f
		, L"���� ���ܵ� Į��");

	DWRITE->RenderText(L"�׸��� Texture ũ��" + to_wstring((int)shadow->GetTextureSize()), 
		RECT{ 200,155,static_cast<LONG>(App.GetWidth()),static_cast<LONG>(App.GetHeight()) }, 25.0f
		, L"���� ���ܵ� Į��");
	switch (shadow->GetQuality())
	{
	case 0:
		DWRITE->RenderText(L"Texture ���� : MIN_MAG_MIP_POINT", 
			RECT{ 200,185,static_cast<LONG>(App.GetWidth()),static_cast<LONG>(App.GetHeight()) }, 25.0f
			, L"���� ���ܵ� Į��");
		break;
	case 1:
		DWRITE->RenderText(L"Texture ���� : MIN_MAG_MIP_LINEAR", 
			RECT{ 200,185,static_cast<LONG>(App.GetWidth()),static_cast<LONG>(App.GetHeight()) }, 25.0f
			, L"���� ���ܵ� Į��");
		break;
	}
}
