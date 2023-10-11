#include "stdafx.h"

Map::Map(){}
Map::~Map(){}

//현재 키보드 입력으로만 벽이나 스위치 작동으로 해놓았음. LateUpdate에서 맞게 수정 필요
void Map::Init()
{
	player = new APlayer;
	player->Init();

	//큐브맵
	sky = Sky::Create();

	//던전 맵
	terrain = Terrain::Create();
	terrain->LoadFile("Floor00.xml");
	terrain->CreateStructuredBuffer();

	//던전 시작 전 입구
	dungeon = Actor::Create();
	dungeon->LoadFile("DungeonStart.xml");

	//던전 클리어 후 입구
	dungeonend = Actor::Create();
	dungeonend->LoadFile("DungeonEnd.xml");

	//던전 벽
	wall = Actor::Create();
	wall->LoadFile("Wall00.xml");

	//게임 시작시 벽
	fieldwall = Actor::Create();
	fieldwall->LoadFile("FieldWall.xml");

	//보스전 진입 바닥
	floor = Actor::Create();
	floor->LoadFile("Floor01.xml");

	//해제용 벽
	LockBlock = Actor::Create();
	LockBlock->LoadFile("LockBlock.xml");

	//톱니바퀴
	saw = Actor::Create();
	saw->LoadFile("SawTooth.xml");

	//다시 돌아올때 작동되는 장애물
	shr = Actor::Create();
	shr->LoadFile("Shuriken.xml");

	//떨어지는 바위
	droprock = Actor::Create();
	droprock->LoadFile("DropRock.xml");

	//보스전 벽
	bosswall = Actor::Create();
	bosswall->LoadFile("BossWall.xml");

	//회전하는 붉은 원
	bossgate = Actor::Create();
	bossgate->LoadFile("BossGate.xml");

	//거대한 기어
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
	//최종보스 문 열기
	if (INPUT->KeyDown(VK_F1))
	{
		dooropen = true;
	}

	if (dooropen == true)
	{
		bossgate->rotation.x += 50.0f * TORADIAN * DELTA;
	}

	//1번 벽 해제
	if (INPUT->KeyDown('1'))
	{
		unlock1 = true;
	}

	if (unlock1 == true)
	{
		LockBlock->Find("Offset")->MoveWorldPos(-UP * 10.0f * DELTA);
		LockBlock->Find("BlockBox")->MoveWorldPos(-UP * 10.0f * DELTA);
	}

	//2번 벽 해제
	if (INPUT->KeyDown('2'))
	{
		unlock2 = true;
	}

	if (unlock2 == true)
	{
		LockBlock->Find("LockBlock1")->MoveWorldPos(-UP * 10.0f * DELTA);
	}

	//3번 벽 해제
	if (INPUT->KeyDown('3'))
	{
		unlock3 = true;
	}

	if (unlock3 == true)
	{
		LockBlock->Find("LockBlock2")->MoveWorldPos(-UP * 10.0f * DELTA);
	}

	//기어 회전
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

	//던전 끝남
	if (INPUT->KeyDown('5'))
	{
		dungeonbreak = true;
	}

	if (dungeonbreak == true)
	{
		dungeon->visible = false;
		dungeonend->visible = true;
	}

	//톱니바퀴
	saw->Find("SawTooth1")->rotation.z -= 120.0f * TORADIAN * DELTA;
	saw->Find("SawTooth2")->rotation.z += 120.0f * TORADIAN * DELTA;
	saw->Find("SawTooth3")->rotation.z -= 120.0f * TORADIAN * DELTA;
	saw->Find("SawTooth4")->rotation.z += 120.0f * TORADIAN * DELTA;

	//1번톱니
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

	//2번톱니
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

	//3번톱니
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

	//4번톱니
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

	//떨어지는 돌
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

	//마지막 표창
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