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
	//�׸��ڰ� ǥ���� ��ġ �����Ű��
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

	// ����
	boss = new MonsterBoss();
	boss->Init();
	//boss->root->SetWorldPos(Vector3(0,0,-250));
	boss->root->SetWorldPos(Vector3(0,0,50));

	boss->GetBoard()->SetObj(player->root);
	//boss->GetBoard()->SetTerrain(map);


	// ����
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
	
	// �׸���
	shadow = new Shadow();
	//shadow2 = new Shadow();
	//ps���̴��� �ٸ����̴�
	RESOURCE->shaders.Load("4.ShadowMap.hlsl");
	RESOURCE->shaders.Load("2.ShadowMap.hlsl");



	//gameui = GameUI::Create();

	// �ƾ���
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
	// �ǰ� ���·� ����
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
		cout <<"����ü�� : " << boss->GetHP() << endl;
	}
	////����&���� ���� ������� ���� ��Ű��
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
	//// ī�޶� �׼� ����
	//if (boss->bState == MonsterBoss::Boss_State::INTRO || boss->bState == MonsterBoss::Boss_State::DEATH)
	//{
	//	// ī�޶� ���� ����� ��ǥ
	//	if (boss->root->anim->GetPlayTime() < 1.0f)
	//	{
	//		Camera::main = (Camera*)boss->root->Find("CameraPos"); // ������ ī�޶� ���� ��ǥ�ϳ������α�
	//	}
	//	// ���� ��Ʈ�ξ����� ī�޶� �׼ǽ���
	//	camAction();
	//}
	//camCheck();

	//Vector3 vtemp = player->GetLocalPos() - boss->root->GetWorldPos();
	//float temp = vtemp.Length();
	//
	//// ��Ʈ�� �̺�Ʈ�� ���ǹ�
	//if (!isintro && introcount > 0)
	//{
	//	if (temp < 200)
	//	{
	//		cout << "���� ��Ʈ�� ���� ����!" << endl;
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
		// ���� ����
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
	// �÷��̾ �׻� ������ �ٶ󺸰� �ֵ��� ����
	// ���� ���ǻ��� : ���� ����� ��ǥ(Vector3)�� �����ؾ��ϹǷ�
	// ��ü���� ������Ʈ �� �Ŀ� ����ϵ��� �մϴ�.
	//lookAtrotation();
	if (!player->root->collider->colOnOff)
	{
		if (TIMER->GetTick(temp, 1.0f))
		{
			cout << "�÷��̾��� �ݶ��̴� ON" << endl;

			player->root->collider->colOnOff = true;
		}
	}

	if (boss->GetHitCol() && player->root->collider->Intersect(boss->GetHitCol()))
	{
		cout << "���� ������ 3" << endl;
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
	// ���� ����
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
	//ī�޶� �;��� ��ġ (������ ī�޶�����Ʈ�� ���� ����ķ��ġ�� ���͸� ���Ѵ�)
	//Vector3 dis = cam->GetWorldPos() - Camera::main->GetWorldPos();
	Vector3 dis = boss->root->Find("CameraPos")->GetWorldPos() - cam->GetWorldPos();

	// ����ķ�� �ƴҶ� ��,������ ��Ŀ���� ������ ķ�϶��� ����ȿ�� ����
	if (Camera::main != cam)
	{
		Camera::main->MoveWorldPos(dis * 0.01f * DELTA); //(��Ÿ�� ������� �ʴ������� ��Į�󰪿� �̹� ����Ǿ��ֱ⶧��)
	}

	//���õ� ī�޶� ��ġ���� ������ �ٶ󺸴� ��
	Camera::main->view = Matrix::CreateLookAt(Camera::main->GetWorldPos(),
	    boss->root->GetWorldPos(), Vector3(0, 1, 0));
	 
	//�⺻ ��������
	Camera::main->proj  = Matrix::CreatePerspectiveFieldOfView(
	Camera::main->fov, Camera::main->width / Camera::main->height, Camera::main->nearZ, Camera::main->farZ);
	Camera::main->SetShadow();
}

void SKH_TestScene::lookAtrotation()
{
	// ��ġ��İ���� ��������ִ� �����
	Matrix R = Matrix::CreateLookAt(player->GetWorldPos(), boss->root->GetWorldPos(), Vector3(0, 1, 0));

	// ������� ��ġ��ķ� ��ȯ�Ѵ�.
	R = R.Transpose();

	// ���[matrix]���� -> ���ʹϾ�[Quaternion]���� ��ȯ
	Quaternion q = Quaternion::CreateFromRotationMatrix(R);

	// ���ʹϾ�[Quaternion]���� -> ȸ��[Rotation]���� ����
	player->rotation.y = Util::QuaternionToYawPtichRoll(q).y;
}

void SKH_TestScene::camCheck()
{
	if (Camera::main != cam) // ���� ī�޶� �ٲ������
	{
		camtimer += DELTA;
		if (camtimer >= 3) // 3�� Ÿ�̸�
		{
			camtimer = 0; // Ÿ�̸� �ʱ�ȭ
			Camera::main = cam; // ����ī�޶� �ǵ�����
		}
	}
}
