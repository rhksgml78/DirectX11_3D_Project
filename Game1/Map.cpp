#include "stdafx.h"

Map::Map(){}
Map::~Map(){}

//���� Ű���� �Է����θ� ���̳� ����ġ �۵����� �س�����. LateUpdate���� �°� ���� �ʿ�
void Map::Init()
{
	player = new APlayer;
	player->Init();

	//ť���
	sky = Sky::Create();

	//���� ��
	terrain = Terrain::Create();
	terrain->LoadFile("Floor00.xml");
	terrain->CreateStructuredBuffer();

	//���� ���� �� �Ա�
	dungeon = Actor::Create();
	dungeon->LoadFile("DungeonStart.xml");

	//���� Ŭ���� �� �Ա�
	dungeonend = Actor::Create();
	dungeonend->LoadFile("DungeonEnd.xml");

	//���� ��
	wall = Actor::Create();
	wall->LoadFile("Wall00.xml");

	//���� ���۽� ��
	fieldwall = Actor::Create();
	fieldwall->LoadFile("FieldWall.xml");

	//������ ���� �ٴ�
	floor = Actor::Create();
	floor->LoadFile("Floor01.xml");

	//������ ��
	LockBlock = Actor::Create();
	LockBlock->LoadFile("LockBlock.xml");

	//��Ϲ���
	saw = Actor::Create();
	saw->LoadFile("SawTooth.xml");

	//�ٽ� ���ƿö� �۵��Ǵ� ��ֹ�
	shr = Actor::Create();
	shr->LoadFile("Shuriken.xml");

	//�������� ����
	droprock = Actor::Create();
	droprock->LoadFile("DropRock.xml");

	//������ ��
	bosswall = Actor::Create();
	bosswall->LoadFile("BossWall.xml");

	//ȸ���ϴ� ���� ��
	bossgate = Actor::Create();
	bossgate->LoadFile("BossGate.xml");

	//�Ŵ��� ���
	trap = Actor::Create();
	trap->LoadFile("Gear.xml");
}

void Map::Release()
{
	terrain->Release();
	player->Release();
	dungeon->Release();
	dungeonend->Release();
	wall->Release();
	fieldwall->Release();
	floor->Release();
	LockBlock->Release();
	saw->Release();
	shr->Release();
	droprock->Release();
	bosswall->Release();
	bossgate->Release();
	trap->Release();
	sky->Release();
}

void Map::Update()
{
	ImGui::Text("FPS: %d", TIMER->GetFramePerSecond());
	ImGui::Begin("Hierarchy");
	
	ImGui::End();

	terrain->Update();
	player->Update();
	dungeon->Update();
	dungeonend->Update();
	floor->Update();
	wall->Update();
	fieldwall->Update();
	LockBlock->Update();
	saw->Update();
	shr->Update();
	droprock->Update();
	bosswall->Update();
	bossgate->Update();
	trap->Update();
	sky->Update();
}

void Map::LateUpdate()
{
	//�������� �� ����
	if (INPUT->KeyDown(VK_F1))
	{
		dooropen = true;
	}

	if (dooropen == true)
	{
		bossgate->rotation.x += 50.0f * TORADIAN * DELTA;
	}

	//1�� �� ����
	if (INPUT->KeyDown('1'))
	{
		unlock1 = true;
	}

	if (unlock1 == true)
	{
		LockBlock->Find("Offset")->MoveWorldPos(-UP * 10.0f * DELTA);
		LockBlock->Find("BlockBox")->MoveWorldPos(-UP * 10.0f * DELTA);
	}

	//2�� �� ����
	if (INPUT->KeyDown('2'))
	{
		unlock2 = true;
	}

	if (unlock2 == true)
	{
		LockBlock->Find("LockBlock1")->MoveWorldPos(-UP * 10.0f * DELTA);
	}

	//3�� �� ����
	if (INPUT->KeyDown('3'))
	{
		unlock3 = true;
	}

	if (unlock3 == true)
	{
		LockBlock->Find("LockBlock2")->MoveWorldPos(-UP * 10.0f * DELTA);
	}

	//��� ȸ��
	if (INPUT->KeyDown('4'))
	{
		gotrap = true;
	}

	if (gotrap == true)
	{
		trap->visible = true;
		trap->rotation.z -= 50.0f * TORADIAN * DELTA;
		trap->MoveWorldPos(RIGHT * 60.0f * DELTA);
	}

	if (trap->GetWorldPos().x >= 900.0f)
	{
		trap->SetLocalPosY(-1000.0f);
		trap->visible = false;
		gotrap = false;
	}

	//���� ����
	if (INPUT->KeyDown('5'))
	{
		dungeonbreak = true;
	}

	if (dungeonbreak == true)
	{
		dungeon->visible = false;
		dungeonend->visible = true;
	}

	//��Ϲ���
	saw->Find("SawTooth1")->rotation.z -= 120.0f * TORADIAN * DELTA;
	saw->Find("SawTooth2")->rotation.z += 120.0f * TORADIAN * DELTA;
	saw->Find("SawTooth3")->rotation.z -= 120.0f * TORADIAN * DELTA;
	saw->Find("SawTooth4")->rotation.z += 120.0f * TORADIAN * DELTA;

	//1�����
	if (INPUT->KeyDown('6'))
	{
		saw->Find("SawTooth1")->SetLocalPosY(40.0f);
	}

	if (saw->Find("SawTooth1")->GetLocalPos().y == 40.0f)
	{
		saw->Find("SawTooth1")->MoveLocalPos(RIGHT * SawSpeed * DELTA);
	}

	if (saw->Find("SawTooth1")->GetLocalPos().x >= 175.0f)
	{
		saw->Find("SawTooth1")->SetLocalPosX(175.0f);
	}

	//2�����
	if (INPUT->KeyDown('7'))
	{
		saw->Find("SawTooth2")->SetLocalPosY(40.0f);
	}

	if (saw->Find("SawTooth2")->GetLocalPos().y == 40.0f)
	{
		saw->Find("SawTooth2")->MoveLocalPos(-RIGHT * SawSpeed * DELTA);
	}

	if (saw->Find("SawTooth2")->GetLocalPos().x <= -45.0f)
	{
		saw->Find("SawTooth2")->SetLocalPosX(-45.0f);
	}

	//3�����
	if (INPUT->KeyDown('8'))
	{
		saw->Find("SawTooth3")->SetLocalPosY(40.0f);
	}

	if (saw->Find("SawTooth3")->GetLocalPos().y == 40.0f)
	{
		saw->Find("SawTooth3")->MoveLocalPos(RIGHT * SawSpeed * DELTA);
	}

	if (saw->Find("SawTooth3")->GetLocalPos().x >= 175.0f)
	{
		saw->Find("SawTooth3")->SetLocalPosX(175.0f);
	}

	//4�����
	if (INPUT->KeyDown('9'))
	{
		saw->Find("SawTooth4")->SetLocalPosY(40.0f);
	}

	if (saw->Find("SawTooth4")->GetLocalPos().y == 40.0f)
	{
		saw->Find("SawTooth4")->MoveLocalPos(-RIGHT * SawSpeed * DELTA);
	}

	if (saw->Find("SawTooth4")->GetLocalPos().x <= -45.0f)
	{
		saw->Find("SawTooth4")->SetLocalPosX(-45.0f);
	}

	//�������� ��
	/*if (INPUT->KeyDown(VK_NUMPAD1))
	{
		droprock1 = true;
	}

	if (droprock1 == true)
	{
		droprock->Find("DropRock1")->visible = true;
		droprock->Find("DropRock1")->MoveWorldPos(-UP * RockSpeed * DELTA);
	}

	if (INPUT->KeyDown(VK_NUMPAD2))
	{
		droprock2 = true;
	}

	if (droprock2 == true)
	{
		droprock->Find("DropRock2")->visible = true;
		droprock->Find("DropRock2")->MoveWorldPos(-UP * RockSpeed * DELTA);
	}

	if (INPUT->KeyDown(VK_NUMPAD3))
	{
		droprock3 = true;
	}

	if (droprock3 == true)
	{
		droprock->Find("DropRock3")->visible = true;
		droprock->Find("DropRock3")->MoveWorldPos(-UP * RockSpeed * DELTA);
	}*/

	//������ ǥâ
	if (INPUT->KeyDown(VK_NUMPAD4))
	{
		shr->visible = true;
	}

	if (shr->visible == true)
	{
		shr->rotation.z += 250.0f * TORADIAN * DELTA;
		shr->MoveWorldPos(FORWARD * 40.0f * DELTA);
	}

	if (shr->GetLocalPos().z >= 0.0f)
	{
		shr->SetWorldPosY(-200.0f);
		shr->visible = false;
	}
}

void Map::RenderHierarchy()
{
	ImGui::Text("FPS: %d", TIMER->GetFramePerSecond());
	ImGui::Begin("Hierarchy");
	terrain->RenderHierarchy();
	dungeon->RenderHierarchy();
	dungeonend->RenderHierarchy();
	floor->RenderHierarchy();
	wall->RenderHierarchy();
	fieldwall->RenderHierarchy();
	LockBlock->RenderHierarchy();
	saw->RenderHierarchy();
	shr->RenderHierarchy();
	droprock->RenderHierarchy();
	bosswall->RenderHierarchy();
	bossgate->RenderHierarchy();
	trap->RenderHierarchy();
	sky->RenderHierarchy();
	ImGui::End();
}

void Map::Render()
{
	sky->Render();
	terrain->Render();
	dungeon->Render();
	dungeonend->Render();
	floor->Render();
	wall->Render();
	fieldwall->Render();
	LockBlock->Render();
	saw->Render();
	shr->Render();
	droprock->Render();
	bosswall->Render();
	bossgate->Render();
	trap->Render();
}

void Map::PreRender()
{
	LIGHT->Set();
}

void Map::ResizeScreen(){}