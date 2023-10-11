#include "stdafx.h"

extern string initObject;
extern int initCount;

Scene2::Scene2()
{
}

Scene2::~Scene2()
{
 
}

void Scene2::Init()
{
    initCount = 0;
    cam = Camera::Create();
    cam->LoadFile("BossSceneCam.xml");
    Camera::main = cam;

    sky = Sky::Create();
    initObject = "sky"; initCount++;

    gameui = GameUI::Create();

    map = new Map();
    map->Init();
    initObject = "Map"; initCount++;

    player = new APlayer();
    player->Init();
    player->root->SetWorldPosZ(1700.0f);

    initObject = "Player"; initCount++;
    

    // ����
    for (int i = 0; i < MONSTERNUM; i++)
    {
        float tempx = RANDOM->Float(625.0f, 875.0f);
        float tempz = RANDOM->Float(-2000.0f, -1750.0f);
        float temprot = RANDOM->Float(0, PI);
        monster2[i] = new MonsterType2();
        monster2[i]->Init();
        monster2[i]->SetPos(Vector3(tempx, 0, tempz));
        monster2[i]->root->rotation.y = temprot;
        monster2[i]->GetBoard()->SetObj(player->root);
        //monster2[i]->GetBoard()->SetTerrain(map);
    }
    for (int i = 0; i < MONSTERNUM; i++)
    {
        float tempx = RANDOM->Float(250.0f, 500.0f);
        float tempz = RANDOM->Float(-500.0f, -250.0f);
        float temprot = RANDOM->Float(0.0f, PI);
        monster[i] = new MonsterType1();
        monster[i]->Init();
        monster[i]->SetPos(Vector3(tempx, 0, tempz));
        monster[i]->root->rotation.y = temprot;
        monster[i]->GetBoard()->SetObj(player->root);
    }
    for (int i = 0; i < MONSTERNUM; i++)
    {
        float tempx = RANDOM->Float(-500.0f, -250.0f);
        float tempz = RANDOM->Float(-875.0f, -625.0f);
        float temprot = RANDOM->Float(0.0f, PI);
        monster3[i] = new MonsterType1();
        monster3[i]->Init();
        monster3[i]->SetPos(Vector3(tempx, 0, tempz));
        monster3[i]->root->rotation.y = temprot;
        monster3[i]->GetBoard()->SetObj(player->root);
        //monster[i]->GetBoard()->SetTerrain(map);
    }

    // �׸���
    shadow = new Shadow();
    //ps���̴��� �ٸ����̴�
    RESOURCE->shaders.Load("4.ShadowMap.hlsl");
    RESOURCE->shaders.Load("2.ShadowMap.hlsl");

    initObject = "Cam"; initCount++;
}

void Scene2::Release()
{
    //��������
    map->Release();

    SafeDelete(shadow);
    for (int i = 0; i < MONSTERNUM; i++)
    {
        SafeDelete(monster[i]);
        SafeDelete(monster2[i]);
        SafeDelete(monster3[i]);
    }
    player->Release();
    gameui->Release();
    cam->Release();

    //t1->join(); //���������� ��ٸ�
    //delete t1;
}


void Scene2::Update()
{
    if (SCENE->GetScene("SC2")->state == SceneState::FADEIN)
    {
        post->psp.color.x += 0.5 / 2.0f * DELTA;
        post->psp.color.y += 0.5 / 2.0f * DELTA;
        post->psp.color.z += 0.5 / 2.0f * DELTA;
        if (post->psp.color.x >= 0.5)
        {
            SCENE->GetScene("SC2")->state = SceneState::NONE;
        }
        //psp.radius -= 600.0f * DELTA;
    }

    if (CamisPlayer) CamSetPlayer();

    if (player->root->GetWorldPos().y != 0)
    {
        player->root->SetWorldPosY(0);
    }

    //Hierarchy
    if (ImGui::Begin("Hierarchy"))
    {
        Camera::main->RenderHierarchy();
        LIGHT->RenderDetail();

        post->RenderDetail();
        gameui->RenderHierarchy();
        sky->RenderHierarchy();
        player->RenderHierarchy();
        map->RenderHierarchy();
        ImGui::End();
    }

    Camera::main->Update();
    map->Update();
    gameui->Update(player);
    sky->Update();
    post->Update();
    player->Update();
    for (int i = 0; i < MONSTERNUM; i++)
    {
        monster[i]->Update();
        monster2[i]->Update();
        monster3[i]->Update();
    }
}

void Scene2::LateUpdate()
{
    // �浹ó�� �ؾ��Ѵ�
    map->LateUpdate();

    if (PlayerAttaks()) // �÷��̾��� ����� ���ݰ��� ����϶�
    {
        // ���� 1, 2, 3 �浹ó��
        for (int i = 0; i < MONSTERNUM; i++)
        {
            if (monster[i]->GetHitCol()->colOnOff) // ���� Ÿ��1�� �ǰ� �ݶ��̴��� ���� ������
            {
                if (player->GetAktCol1()->Intersect(monster[i]->GetHitCol())) // �����ݶ��̴� �浹��
                    monster[i]->GetDameged(player->GetPower());
                
            }

            if (monster[0]->hp <= 0) monCount1--;
            if (monster[1]->hp <= 0) monCount1--;
            if (monster[2]->hp <= 0) monCount1--;
            if (monCount1 <= 0)
            {
                map->LockBlock->Find("LockBlock1")->MoveWorldPos(-UP * 10.0f * DELTA);
            }

            if (monster2[i]->GetHitCol()->colOnOff) // ���� Ÿ��2�� �ǰ� �ݶ��̴��� ���� ������
            {
                if (player->GetAktCol1()->Intersect(monster2[i]->GetHitCol())) // �����ݶ��̴� �浹��
                    monster2[i]->GetDameged(player->GetPower());
            }

            if (monster2[0]->hp <= 0) monCount2--;
            if (monster2[1]->hp <= 0) monCount2--;
            if (monster2[2]->hp <= 0) monCount2--;
            if (monCount2 <= 0) map->dooropen = true;

            if (monster3[i]->GetHitCol()->colOnOff) // ���� Ÿ��1�� �ǰ� �ݶ��̴��� ���� ������
            {
                if (player->GetAktCol1()->Intersect(monster3[i]->GetHitCol())) // �����ݶ��̴� �浹��
                    monster3[i]->GetDameged(player->GetPower());
            }

            if (monster3[0]->hp <= 0)  monCount3--;
            if (monster3[1]->hp <= 0)  monCount3--;
            if (monster3[2]->hp <= 0)  monCount3--;
            if (monCount3 <= 0)
            {
                map->LockBlock->Find("LockBlock2")->MoveWorldPos(-UP * 10.0f * DELTA);
                map->gotrap = true;
            }
        }
    }

    // ���� ���ݻ����϶�
    if (MonsterAttacks()) // ���� 1,3 ���ݻ����϶�
    {
        if (player->GetHitCol()->colOnOff && // �÷��̾��� �ǰ� �ݶ��̴��� �����ְ�
            player->root->anim->GetPlayNum() != 27) // ��������°� �ƴҶ�
        {
            for (int i = 0; i < MONSTERNUM; i++)
            {
                if (monster[i]->GetAktCol1()->Intersect(player->GetHitCol()) ||
                    monster[i]->GetAktCol2()->Intersect(player->GetHitCol()) ||
                    monster[i]->GetAktCol3()->Intersect(player->GetHitCol()))
                {
                    player->GetDameged(monster[i]->GetPower());
                }

                if (monster3[i]->GetAktCol1()->Intersect(player->GetHitCol()) ||
                    monster3[i]->GetAktCol2()->Intersect(player->GetHitCol()) ||
                    monster3[i]->GetAktCol3()->Intersect(player->GetHitCol()))
                {
                    player->GetDameged(monster3[i]->GetPower());
                }
            }
        }
    }
    if (Monster2Attacks())
    {
        if (player->GetHitCol()->colOnOff && // �÷��̾��� �ǰ� �ݶ��̴��� �����ְ�
            player->root->anim->GetPlayNum() != 27) // ��������°� �ƴҶ�
        {
            for (int i = 0; i < MONSTERNUM; i++)
            {
                if (monster2[i]->GetAktCol1()->Intersect(player->GetHitCol()) ||
                    monster2[i]->GetAktCol2()->Intersect(player->GetHitCol()))
                {
                    player->GetDameged(monster2[i]->GetPower());
                }
            }
        }
    }

    //��Ϲ���
    map->saw->Find("SawTooth1")->rotation.z -= 120.0f * TORADIAN * DELTA;
    map->saw->Find("SawTooth2")->rotation.z += 120.0f * TORADIAN * DELTA;
    map->saw->Find("SawTooth3")->rotation.z -= 120.0f * TORADIAN * DELTA;
    map->saw->Find("SawTooth4")->rotation.z += 120.0f * TORADIAN * DELTA;

    //1�����
    if (player->GetHitCol()->Intersect(map->terrain->Find("ObLine1")->collider))
    {
        map->saw->Find("SawTooth1")->SetLocalPosY(40.0f);
    }

    if (map->saw->Find("SawTooth1")->GetLocalPos().y == 40.0f)
    {
        map->saw->Find("SawTooth1")->MoveLocalPos(RIGHT * map->SawSpeed * DELTA);
    }

    if (map->saw->Find("SawTooth1")->GetLocalPos().x >= 175.0f)
    {
        map->saw->Find("SawTooth1")->SetLocalPosX(175.0f);
    }

    //2�����
    if (player->GetHitCol()->Intersect(map->terrain->Find("ObLine2")->collider))
    {
        map->saw->Find("SawTooth2")->SetLocalPosY(40.0f);
    }

    if (map->saw->Find("SawTooth2")->GetLocalPos().y == 40.0f)
    {
        map->saw->Find("SawTooth2")->MoveLocalPos(-RIGHT * map->SawSpeed * DELTA);
    }

    if (map->saw->Find("SawTooth2")->GetLocalPos().x <= -45.0f)
    {
        map->saw->Find("SawTooth2")->SetLocalPosX(-45.0f);
    }

    //3�����
    if (player->GetHitCol()->Intersect(map->terrain->Find("ObLine3")->collider))
    {
        map->saw->Find("SawTooth3")->SetLocalPosY(40.0f);
    }

    if (map->saw->Find("SawTooth3")->GetLocalPos().y == 40.0f)
    {
        map->saw->Find("SawTooth3")->MoveLocalPos(RIGHT * map->SawSpeed * DELTA);
    }

    if (map->saw->Find("SawTooth3")->GetLocalPos().x >= 175.0f)
    {
        map->saw->Find("SawTooth3")->SetLocalPosX(175.0f);
    }

    //4�����
    if (player->GetHitCol()->Intersect(map->terrain->Find("ObLine4")->collider))
    {
        map->saw->Find("SawTooth4")->SetLocalPosY(40.0f);
    }

    if (map->saw->Find("SawTooth4")->GetLocalPos().y == 40.0f)
    {
        map->saw->Find("SawTooth4")->MoveLocalPos(-RIGHT * map->SawSpeed * DELTA);
    }

    if (map->saw->Find("SawTooth4")->GetLocalPos().x <= -45.0f)
    {
        map->saw->Find("SawTooth4")->SetLocalPosX(-45.0f);
    }

    if (map->dooropen == true)
    {
        map->bossgate->rotation.x += 50.0f * TORADIAN * DELTA;
        map->terrain->Find("FinalTrapLine")->collider->SetLocalPosY(3.4f);
        map->terrain->Find("ReadyBoss")->collider->SetLocalPosY(30.0f);
    }

    //������ ��ֹ�
    if (player->GetHitCol()->Intersect(map->terrain->Find("FinalTrapLine")->collider))
    {
        map->shr->visible = true;
    }

    if (map->shr->visible == true)
    {
        map->shr->rotation.z += 250.0f * TORADIAN * DELTA;
        map->shr->MoveWorldPos(FORWARD * 10.0f * DELTA);
    }

    if (map->shr->GetLocalPos().z >= 0.0f)
    {
        map->shr->SetWorldPosY(-200.0f);
        map->shr->visible = false;
    }

    //���� �� ���ư���
    if (player->GetHitCol()->Intersect(map->terrain->Find("ReadyBoss")->collider))
    {
        player->root->SetWorldPosZ(2000.0f);
        map->GoBoss = true;
    }

    if (map->GoBoss == true && player->GetHitCol()->Intersect(map->floor->Find("FinalBattleStart")->collider))
    {
        player->root->SetLocalPos(Vector3(600.0f, 400.0f, 1770.0f));
        map->BossPattern = true;
    }

    if (map->BossPattern == true)
    {
        //���⿡ �Է�
    }


    if ((player->GetHitCol()->Intersect(map->terrain->Find("RockLine1")->collider)))
    {
        map->droprock3 = true;
    }

    if (map->droprock3 == true)
    {
        map->droprock->Find("DropRock3")->visible = true;
        map->droprock->Find("DropRock3")->MoveWorldPos(-UP * map->RockSpeed * DELTA);
    }

    if ((player->GetHitCol()->Intersect(map->terrain->Find("RockLine2")->collider)))
    {
        map->droprock2 = true;
    }

    if (map->droprock2 == true)
    {
        map->droprock->Find("DropRock2")->visible = true;
        map->droprock->Find("DropRock2")->MoveWorldPos(-UP * map->RockSpeed * DELTA);
    }

    if ((player->GetHitCol()->Intersect(map->terrain->Find("RockLine3")->collider)))
    {
        map->droprock1 = true;
    }

    if (map->droprock1 == true)
    {
        map->droprock->Find("DropRock1")->visible = true;
        map->droprock->Find("DropRock1")->MoveWorldPos(-UP * map->RockSpeed * DELTA);
    }

    //1������
    //���� ���԰� ���ÿ� 1�� �� ����
    if (player->GetHitCol()->Intersect(map->dungeon->Find("GoDungeon")->collider))
    {
        player->root->SetWorldPosZ(0.0f);
        map->unlock1 = true;
        map->dungeon->visible = false;
        map->dungeonend->visible = true;
    }

    if (map->unlock1 == true)
    {
        map->LockBlock->Find("Offset")->MoveWorldPos(-UP * 10.0f * DELTA);
        map->LockBlock->Find("BlockBox")->MoveWorldPos(-UP * 10.0f * DELTA);
    }


}

void Scene2::PreRender()
{
    post->SetTarget();
    LIGHT->Set();
    Camera::main->Set(); // ī�޶� ����
    //ù��° ����Ÿ��
    sky->Render();
    map->Render();
    player->Render();
    for (int i = 0; i < MONSTERNUM; i++)
    {
        monster[i]->Render();
        monster2[i]->Render();
        monster3[i]->Render();
    }
}

void Scene2::Render()
{
    post->Render();
    gameui->Render();
    shadow->SetTarget(player->root->GetWorldPos());
    player->root->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
    map->terrain->Render(RESOURCE->shaders.Load("5.ShadowMap.hlsl"));
    for (int i = 0; i < MONSTERNUM; i++)
    {
        monster[i]->root->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
        monster2[i]->root->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
        monster3[i]->root->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
    }
    shadow->SetRGBTexture(7); // �׸��� �ؽ�ó ����

    DWRITE->RenderText(L"FPS : " + to_wstring(TIMER->GetFramePerSecond()), RECT{ 5,5,500,500 }, 40.0f);
}

void Scene2::ResizeScreen()
{
    Camera::main->width = App.GetWidth();
    Camera::main->height = App.GetHeight();
    Camera::main->viewport.width = App.GetWidth();
    Camera::main->viewport.height = App.GetHeight();
}

void Scene2::CamSetPlayer()
{
    cam->SetWorldPos(player->root->Find("Campoint")->GetWorldPos());
    cam->rotation.y = player->root->Find("Offset")->rotation.y - 180 * TORADIAN;
}

bool Scene2::PlayerAttaks()
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
        if (player->root->anim->GetPlayTime() > 0.3 &&
            player->root->anim->GetPlayTime() < 0.8)
        {
            return true;
        }
    }

    else
        return false;
}

bool Scene2::MonsterAttacks() // �ٰŸ�
{
    for (int i = 0; i < MONSTERNUM; i++)
    {
        if (monster[i]->root->anim->GetPlayNum() > 4 && monster[i]->root->anim->GetPlayNum() < 8) // ���ݸ���϶�
            return true;
        else
            return false;
    }
    for (int i = 0; i < MONSTERNUM; i++)
    {
        if (monster3[i]->root->anim->GetPlayNum() > 4 && monster3[i]->root->anim->GetPlayNum() < 8) // ���ݸ���϶�
            return true;
        else
            return false;
    }
}

bool Scene2::Monster2Attacks() // ���Ÿ�
{
    for (int i = 0; i < MONSTERNUM; i++)
    {
        if (monster2[i]->root->anim->GetPlayNum() > 4 && monster2[i]->root->anim->GetPlayNum() < 8) // ���ݸ���϶�
            return true;
        else
            return false;
    }
}
