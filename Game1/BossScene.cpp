#include "stdafx.h"

// �⺻ ������ �Ҹ���
BossScene::BossScene() { }
BossScene::~BossScene() { }

void BossScene::Init() // �ʱ�ȭ
{
	cam = Camera::Create();
	//cam->LoadFile("BossSceneCam.xml");
	cam->LoadFile("PPTCAMPOS.xml");
	Camera::main = cam;

	sky = Sky::Create(); // ���� default = yokohama.dds
	sky->texCube->LoadFile("sunsetcube1024.dds");

	map = Terrain::Create();
	//map->LoadFile("Terrain.xml");
	map->LoadFile("DJ_Terrain.xml");
	map->showNode = true;
	map->UpdateMeshUv();
	map->CreateStructuredBuffer();
	//�׸��ڰ� ǥ���� ��ġ �����Ű��
	map->material->shadow = 0.5f; // �������������ΰ�
	map->showNode = false;

	for (int i = 0; i < 3; i++) // �ӽ������� ��ֹ� 3�� ��ġ�Ұ��Դϴ�.
	{
		map_stone[i] = Actor::Create("Rock" + to_string(i + 1));
		string temp = "MapRock" + to_string(i + 1) + ".xml";
		map_stone[i]->LoadFile(temp);
	}

	//�ӽ� �÷��̾� - ���� �ܼ��� �����Ӹ� �����ϸ� ī�޶����� ��� �ּ�ó������
	player = new APlayer();
	player->Init();

	gameui = GameUI::Create();

	// ����
	boss = new MonsterBoss();
	boss->Init();
	boss->root->SetWorldPos(Vector3(0,0,-250));
	boss->GetBoard()->SetObj(player->root); // Ʈ�� ���ۿ� ����
	boss->GetBoard()->SetTerrain(map); // Ʈ�� ���ۿ� ����
	player->SetTarget(boss->root);

	// �׽�Ʈ�� ����
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

	// �׸��� 2,4 �׸��ڸ��ο� ���̴����� �ε�
	shadow = new Shadow();
	RESOURCE->shaders.Load("4.ShadowMap.hlsl");
	RESOURCE->shaders.Load("2.ShadowMap.hlsl");

	// ��� ȭ�� ũ�� ������
	ResizeScreen();

	// �ƾ� ����
	isintro = false;
	Camera::mainCamSpeed = 100.0f;
}

void BossScene::Release()
{
	// ���� ������ �Ҵ������ �ݴ�
	SafeDelete(shadow);

	// �׽�Ʈ�� ����
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
void BossScene::Hi() // GUI ������ Ȯ��
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
void BossScene::CamSetPlayer() // �÷��̾��� CAMȿ���� ó�����������Ƿ� ������ ��ġ�� ����
{
	cam->SetWorldPos(player->root->Find("Campoint")->GetWorldPos());
	cam->rotation.y = player->root->Find("Offset")->rotation.y - 180*TORADIAN;
}

void BossScene::Update()
{
	for (int i = 0; i < 3; i++) // �ӽ������� ��ֹ� 3�� ��ġ�Ұ��Դϴ�.
	{
		Vector3 loadpos;
		loadpos = map->root->Find("COL" + to_string(i + 1))->GetWorldPos();
		map_stone[i]->SetWorldPos(loadpos);
	}

	// ���� �ֵ� ������Ʈ �����
	Camera::ControlMainCam(); // ī�޶� ���� �����̰�ʹٸ� �ּ�����
	ImGui::SliderFloat("CAMSPEED",&Camera::mainCamSpeed, 0.0f, 100.0f);
	//if (CamisPlayer) CamSetPlayer(); // �ƾ��̾ƴҋ��� ī�޶� �÷��̾ ����
	//if (!iscutscene) CutScene(); // �ƾ��� �ѹ��� ������ߴٸ� ��� ����

	// �÷��̾� Y�� ���Ʋ����. �ӽÿ����� ���� �÷��̾� ����� ���� �ľ� �ʿ�
	if (player->root->GetLocalPos().y != 0.0f)
		player->root->SetLocalPosY(0);


	// �ǰݿ� ���� ����üũ�� �ӽ� ���
	if (INPUT->KeyDown(VK_NUMPAD4))
	{
		boss->GetDameged(10);
	}

	Hi(); // ���̾����Ű

	// ������Ʈ �ϰ� ����
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
	// �浹�˻糪 ��ȣ�ۿ���� Late���� ����
	// ���� �浹�˻�� ���� ������ ���� ä��� ������ ����

#pragma region Player Attacks
	if (PlayerAttaks()) // �÷��̾��� ����� ���ݰ��� ����϶�
	{
		if (boss->GetHitCol()->colOnOff) // ������ �ǰ� �ݶ��̴��� ���� ������
		{
			if (player->GetAktCol1()->Intersect(boss->GetHitCol())) // �����ݶ��̴� �浹��
			{
				//cout << "���� ���� " << player->GetPower() << " �� ������!" << endl;
				boss->GetDameged(player->GetPower());
			}
		}

		if (monster->GetHitCol()->colOnOff) // ���� Ÿ��1�� �ǰ� �ݶ��̴��� ���� ������
		{
			if (player->GetAktCol1()->Intersect(monster->GetHitCol())) // �����ݶ��̴� �浹��
			{
				//cout << "���� Ÿ��1 ���� " << player->GetPower() << " �� ������!" << endl;
				monster->GetDameged(player->GetPower());
			}
		}

		if (monster2->GetHitCol()->colOnOff) // ���� Ÿ��2�� �ǰ� �ݶ��̴��� ���� ������
		{
			if (player->GetAktCol1()->Intersect(monster2->GetHitCol())) // �����ݶ��̴� �浹��
			{
				//cout << "���� Ÿ��2 ���� " << player->GetPower() << " �� ������!" << endl;
				monster2->GetDameged(player->GetPower());
			}
		}
	}
#pragma endregion

#pragma region Boss Attacks
	//if (BossAttacks()) // ������ ����� ���ݰ��� ����϶�
	//{ 
	//	if (player->GetHitCol()->colOnOff && // �÷��̾��� �ǰ� �ݶ��̴��� �����ְ�
	//		player->root->anim->GetPlayNum() != 27) // ��������°� �ƴҶ�
	//	{
	//		if (boss->GetAktCol1()->Intersect(player->GetHitCol()))
	//		{
	//			//cout << "�÷��̾� ���� " << boss->GetPower() << " �� ������!" << endl;
	//			player->GetDameged(boss->GetPower());
	//		}
	//		if (boss->GetAktCol2()->Intersect(player->GetHitCol()))
	//		{
	//			//cout << "�÷��̾� ���� " << boss->GetPower() << " �� ������!" << endl;
	//			player->GetDameged(boss->GetPower());
	//		}
	//	}
	//}

	//if (BossSkillAttacks()) // ������ ��ų �������϶�
	//{
	//	if (player->GetHitCol()->colOnOff && // �÷��̾��� �ǰ� �ݶ��̴��� �����ְ�
	//		player->root->anim->GetPlayNum() != 27) // ��������°� �ƴҶ�
	//	{
	//		if (boss->GetSkillCol1()->Intersect(player->GetHitCol()))
	//		{
	//			//cout<<"�÷��̾� ���� " << boss->GetPower() << " �� ������!" << endl;
	//			player->GetDameged(boss->GetPower());
	//		}
	//		if (boss->GetSkillCol2()->Intersect(player->GetHitCol()))
	//		{
	//			//cout << "�÷��̾� ���� " << boss->GetPower() << " �� ������!" << endl;
	//			player->GetDameged(boss->GetPower());
	//		}
	//	}
	//}
#pragma endregion

#pragma region Monster Attacks
	//if (MonsterAttacks()) // ���� 1��Ÿ���� ���ݻ����϶�
	//{
	//	if (player->GetHitCol()->colOnOff && // �÷��̾��� �ǰ� �ݶ��̴��� �����ְ�
	//		player->root->anim->GetPlayNum() != 27) // ��������°� �ƴҶ�
	//	{
	//		// ������ ���� �ݶ��̴����� �÷��̾��� ��Ʈ�ݶ��̴��� �浹������
	//		// ����1�� Ÿ���� �ݶ��̴��� �޼�, ������, �� 3���̴�!
	//		if (monster->GetAktCol1()->Intersect(player->GetHitCol()) ||
	//			monster->GetAktCol2()->Intersect(player->GetHitCol()) ||
	//			monster->GetAktCol3()->Intersect(player->GetHitCol()))
	//		{
	//			cout << "�÷��̾� ���� [���� 1��]" << monster->GetPower() << " �� ������!" << endl;
	//			player->GetDameged(monster->GetPower());
	//		}
	//	}
	//}
#pragma endregion

#pragma region Monster2 Attacks
	//if (Monster2Attacks()) // ���� 2��Ÿ���� ���ݻ����϶�
	//{
	//	if (player->GetHitCol()->colOnOff && // �÷��̾��� �ǰ� �ݶ��̴��� �����ְ�
	//		player->root->anim->GetPlayNum() != 27) // ��������°� �ƴҶ�
	//	{
	//		// ������ ���� �ݶ��̴����� �÷��̾��� ��Ʈ�ݶ��̴��� �浹������
	//		// ����2�� Ÿ���� �ݶ��̴��� ��ų1, ��ų2 ���̴�!
	//		if (monster2->GetAktCol1()->Intersect(player->GetHitCol()) ||
	//			monster2->GetAktCol2()->Intersect(player->GetHitCol()))
	//		{
	//			cout << "�÷��̾� ���� [���� 2��] ��ų " << monster2->GetPower() << " �� ������!" << endl;
	//			player->GetDameged(monster2->GetPower());
	//		}
	//	}
	//}
#pragma endregion
}

void BossScene::PreRender() // 1�� ���� (Ư���� ������ ���� ���� �κ�)
{
	// RTT �������� ���� Ư��ȿ���� �����ų�� ���� Ȯ�� �ʿ�(����δ� Ư��ȿ������X)
	
	// �׸���ȿ�� ���� ���� ������
	ShadowShaderSet();
}

void BossScene::Render() // 2�� ���� (PreRender���� ������ ����)
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

	//for (int i = 0; i < 3; i++)
	//	map_stone[i]->Render();

	// ���� ���¿� ���� �ʿ��� UI ���� ����
	if (boss->bState != MonsterBoss::Boss_State::INTRO)
	{
		gameui->Render();
		//if(isintro == true && boss->GetHP() > 0)
		if (boss->bState != MonsterBoss::Boss_State::DEATH) // ���������������� �����߽��ϴ�.
			gameui->GetBossBar()->Render();
	}

	// ȭ�鿡�� ������ Ȯ���ϱ�� (������ 60���Ϸ� �������� �ʵ��� �����ؾ��մϴ�)
	DWRITE->RenderText(L"FRAME : " + to_wstring(TIMER->GetFramePerSecond()), 
		RECT{ 5,5,static_cast<LONG>(App.GetWidth()),static_cast<LONG>(App.GetHeight())});
}

void BossScene::ResizeScreen() // ī�޶� ȭ�� ũ�� ����
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

	// ��Ʈ�� �̺�Ʈ�� ���ǹ�
	if (!isintro && introcount > 0)
	{
		if (temp < 200)
		{
			//cout << "���� ��Ʈ�� ���� ����!" << endl;
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

	// ī�޶� �׼� ����
	if (boss->bState == MonsterBoss::Boss_State::INTRO)
	{
		// ī�޶� ���� ����� ��ǥ
		if (boss->root->anim->GetPlayTime() < 1.0f)
		{
			Camera::main = (Camera*)boss->root->Find("CameraPos"); // ������ ī�޶� ���� ��ǥ�ϳ������α�
		}
		// ���� ��Ʈ�ξ����� ī�޶� �׼ǽ���
		camAction();
	}
	camCheck(); // ����ķ�� cam�̾ƴ϶�� 3�ʵ��� ������ ����ķ�� cam���� ��ȯ
}

void BossScene::camAction()
{
	//ī�޶� �;��� ��ġ (������ ī�޶�����Ʈ�� ���� ����ķ��ġ�� ���͸� ���Ѵ�)
	//Vector3 dis = cam->GetWorldPos() - Camera::main->GetWorldPos();
	//Vector3 dis = boss->root->GetWorldPos() - cam->GetWorldPos();
	boss->root->Find("CameraPos")->SetWorldPosY(20);
	Vector3 dis = boss->root->Find("CameraPos")->GetWorldPos() - cam->GetWorldPos();

	// ����ķ�� �ƴҶ� ��,������ ��Ŀ���� ������ ķ�϶��� ����ȿ�� ����
	if (Camera::main != cam)
	{
		Camera::main->MoveWorldPos(dis * 0.025f * DELTA);
	}

	//���õ� ī�޶� ��ġ���� ������ �ٶ󺸴� ��
	Camera::main->view = Matrix::CreateLookAt(Camera::main->GetWorldPos(),
		boss->root->GetWorldPos(), Vector3(0, 1, 0));

	//�⺻ ��������
	Camera::main->proj = Matrix::CreatePerspectiveFieldOfView(
		Camera::main->fov, Camera::main->width / Camera::main->height, Camera::main->nearZ, Camera::main->farZ);
	Camera::main->SetShadow();
}

void BossScene::camCheck()
{
	if (Camera::main != cam) // ���� ī�޶� �ٲ������
	{
		camtimer += DELTA;
		if (camtimer >= 7) //Ÿ�̸�
		{
			camtimer = 0; // Ÿ�̸� �ʱ�ȭ
			Camera::main = cam; // ����ī�޶� �ǵ�����
			cam->LoadFile("BossSceneCam.xml");
			iscutscene = true; // �ƾ��� ���� true��ȯ.
			CamisPlayer = true; // ī�޶� �ٽ� �÷������ ���� ��ȯ
			//�⺻ �������� �缳��
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
	if (boss->bState == MonsterBoss::Boss_State::SKILL) // ��ų���¶��
		return true;
	else
		return false;
}

bool BossScene::MonsterAttacks()
{
	if (monster->root->anim->GetPlayNum() > 4 && monster->root->anim->GetPlayNum() < 8) // ���ݸ���϶�
		return true;
	else
		return false;
}

bool BossScene::Monster2Attacks()
{
	if (monster2->root->anim->GetPlayNum() > 5 && monster2->root->anim->GetPlayNum() < 8) // ���ݸ���϶�
		return true;
	else
		return false;
}

void BossScene::ShadowShaderSet()
{
	shadow->SetTarget(player->root->GetWorldPos()); // �׸��ڴ� �÷��̾� ��������
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
