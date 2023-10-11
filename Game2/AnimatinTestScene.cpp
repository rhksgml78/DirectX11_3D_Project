#include "stdafx.h"

AnimatinTestScene::AnimatinTestScene()
{

    Cam = Camera::Create();
    Cam->LoadFile("Cam.xml");
    Cam2 = Camera::Create();

    Camera::main = Cam;



    grid = Grid::Create();

    test = Actor::Create();
    test->LoadFile("BossMonsterSE.xml");
    //test2 = Actor::Create();
    //test2->LoadFile("Monster1.xml");
    statealllfalse();
    state = 1;
    idle = true;
    test->anim->ChangeAnimation(AnimationState::LOOP, state);
    fontsize = 40;
    fontrect = { 10,10,1000,1000 };
    fontstyle = L"돋움체";
    test->Find("Warrok")->material->ambient = Color(0.5f, 0.5f, 0.5f, 1.0f);
    test->Find("Warrok")->material->diffuse = Color(0.5f, 0.5f, 0.5f, 1.0f);
    test->Find("Warrok")->material->specular = Color(1.0f, 1.0f, 1.0f, 1.0f);
    test->Find("Warrok")->material->emissive = Color(1.0f, 1.0f, 1.0f, 1.0f);

    boss_trail = new SlashTrail();
    boss_trail->Top = test->Find("RightPoint1");
    boss_trail->Bottom = test->Find("RightPoint2");
    boss_trail->interval = 0.0f;
    boss_trail->maxTrail = 20;
}

AnimatinTestScene::~AnimatinTestScene()
{

    Cam->Release();
}

void AnimatinTestScene::Init()
{
}

void AnimatinTestScene::Release()
{

}


void AnimatinTestScene::Update()
{
    LIGHT->RenderDetail();
    if (state > 5 and state < 12)
    {
        // L - 8 9 11   R - 7 10 6
        if (state == 8 || state == 9)
        {
            boss_trail->Top = test->Find("LeftPoint1");
            boss_trail->Bottom = test->Find("LeftPoint2");
        }
        else 
        {
            boss_trail->Top = test->Find("RightPoint1");
            boss_trail->Bottom = test->Find("RightPoint2");
        }

        if (not boss_trail->isPlaying)
        {
            if (test->anim->GetPlayTime() > 0.1 and test->anim->GetPlayTime() < 0.7)
            {
                boss_trail->Play();
            }
        }
        else
        {
            if (test->anim->GetPlayTime() > 0.7)
            {
                boss_trail->Stop();
            }
        }
    }
    else
    {
        boss_trail->Stop();

    }



    Camera::main->width = App.GetWidth();
    Camera::main->height = App.GetHeight();
    Camera::main->viewport.width = App.GetWidth();
    Camera::main->viewport.height = App.GetHeight();

    Camera::main->Update();
    Camera::ControlMainCam();


    ImGui::Text("FPS: %d", TIMER->GetFramePerSecond());

    ImGui::Begin("Hierarchy");
    test->RenderHierarchy();
    //test2->RenderHierarchy();
    Cam->RenderHierarchy();
    Cam2->RenderHierarchy();
    ImGui::End();

    ImGui::Begin("Trail_Effect");
    //보스 공격시 보일 트레일
    boss_trail->RenderDetail();
    ImGui::End();


    if (INPUT->KeyDown(VK_NUMPAD0))
    {
        statealllfalse();
        roar = true;
        state = 0;
        test->anim->ChangeAnimation(AnimationState::ONCE_LAST, state);
    }
    if (INPUT->KeyDown(VK_NUMPAD1))
    {
        statealllfalse();
        idle = true;
        state = 1;
        test->anim->ChangeAnimation(AnimationState::LOOP, state);
    }
    if (INPUT->KeyDown(VK_NUMPAD2))
    {
        statealllfalse();
        walk = true;
        state = 2;
        test->anim->ChangeAnimation(AnimationState::LOOP, state);
    }
    if (INPUT->KeyDown(VK_NUMPAD3))
    {
        statealllfalse();
        run = true;
        state = 3;
        test->anim->ChangeAnimation(AnimationState::LOOP, state);
    }

    if (INPUT->KeyDown(VK_NUMPAD4)) // 약공격
    {
        boss_trail->Top = test->Find("RightPoint1");
        boss_trail->Bottom = test->Find("RightPoint1");
        statealllfalse();
        attack1 = true;
        state = 7;

        test->anim->ChangeAnimation(AnimationState::ONCE_LAST, state);
    }
    if (attack1)
    {
        if (state == 7 && test->anim->GetPlayTime() >= 1.0f)
        {

            state++;
            test->anim->ChangeAnimation(AnimationState::ONCE_LAST, state);
        }
    }

    if (INPUT->KeyDown(VK_NUMPAD5)) // 강공격
    {
        statealllfalse();
        attack2 = true;
        state = 9;
        test->anim->ChangeAnimation(AnimationState::ONCE_LAST, state);
    }
    if (attack2)
    {
        if (state == 9 && test->anim->GetPlayTime() >= 1.0f)
        {
            state++;
            test->anim->ChangeAnimation(AnimationState::ONCE_LAST, state);
        }
    }

    if (INPUT->KeyDown(VK_NUMPAD6))
    {
        statealllfalse();
        attack3 = true;
        state = 11;
        boss_trail->Top = test->Find("LeftPoint1");
        boss_trail->Bottom = test->Find("LeftPoint2");
        test->anim->ChangeAnimation(AnimationState::ONCE_LAST, state);
    }

    if (INPUT->KeyDown(VK_NUMPAD7)) // 패링 성공
    {
        statealllfalse();
        parry = true;
        state = 14;
        test->anim->ChangeAnimation(AnimationState::ONCE_LAST, state);
    }
    if (parry)
    {
        if (state == 14 && test->anim->GetPlayTime() >= 1.0f)
        {
            state++;
            test->anim->ChangeAnimation(AnimationState::ONCE_LAST, state);
        }
        if (state == 15 && test->anim->GetPlayTime() >= 1.0f)
        {
            state++;
            test->anim->ChangeAnimation(AnimationState::ONCE_LAST, state);
        }
        if (state == 16 && test->anim->GetPlayTime() >= 1.0f)
        {
            state++;
            test->anim->ChangeAnimation(AnimationState::ONCE_LAST, state);
        }
        if (state == 17 && test->anim->GetPlayTime() >= 1.0f)
        {
            state++;
            test->anim->ChangeAnimation(AnimationState::ONCE_LAST, state);
        }
    }


    if (INPUT->KeyDown(VK_NUMPAD8)) // 패링 실패
    {
        statealllfalse();
        parryfail = true;
        state = 14;
        test->anim->ChangeAnimation(AnimationState::ONCE_LAST, state);
    }
    if (parryfail)
    {
        if (state == 14 && test->anim->GetPlayTime() >= 1.0f)
        {
            state++;
            test->anim->ChangeAnimation(AnimationState::ONCE_LAST, state);
        }
        if (state == 15 && test->anim->GetPlayTime() >= 1.0f)
        {
            state += 3;
            test->anim->ChangeAnimation(AnimationState::ONCE_LAST, state);
        }
    }

    if (INPUT->KeyDown(VK_NUMPAD9)) // 사망
    {
        statealllfalse();
        death = true;
        state = 12;
        test->anim->ChangeAnimation(AnimationState::ONCE_LAST, state);
    }
    if (death)
    {
        if (state == 12 && test->anim->GetPlayTime() >= 1.0f)
        {
            state++;
            test->anim->ChangeAnimation(AnimationState::ONCE_LAST, state);
        }
    }

    test->Update();
    //test2->Update();
    Cam->Update();
    Cam2->Update();
    boss_trail->Update();
}

void AnimatinTestScene::LateUpdate()
{

}

void AnimatinTestScene::PreRender()
{


}

void AnimatinTestScene::Render()
{
    Camera::main->Set();
    LIGHT->Set();
    grid->Render();
    test->Render();
    boss_trail->Render();
    //test2->Render();
    renderText();


}

void AnimatinTestScene::ResizeScreen()
{
    Cam->width = App.GetWidth();
    Cam->height = App.GetHeight();
    Cam->viewport.width = App.GetWidth();
    Cam->viewport.height = App.GetHeight();
}

void AnimatinTestScene::statealllfalse()
{
    roar = false;
    idle = false;
    walk = false;
    run = false;
    jump = false;
    attack1 = false;
    attack2 = false;
    attack3 = false;
    death = false;
    parry = false;
    parryfail = false;
}

void AnimatinTestScene::renderText()
{
    switch (state)
    {
    case 0:
        DWRITE->RenderText(L"BossState : Roar", fontrect, fontsize, fontstyle, Color(0, 0, 0, 1));
        break;
    case 1:
        DWRITE->RenderText(L"BossState : Idle", fontrect, fontsize, fontstyle, Color(0, 0, 0, 1));
        break;
    case 2:
        DWRITE->RenderText(L"BossState : Walk", fontrect, fontsize, fontstyle, Color(0, 0, 0, 1));
        break;
    case 3:
        DWRITE->RenderText(L"BossState : Run", fontrect, fontsize, fontstyle, Color(0, 0, 0, 1));
        break;
    case 7:
        DWRITE->RenderText(L"BossState : Attack_1_Right", fontrect, fontsize, fontstyle, Color(0, 0, 0, 1));
        break;
    case 8:
        DWRITE->RenderText(L"BossState : Attack_1_Left", fontrect, fontsize, fontstyle, Color(0, 0, 0, 1));
        break;
    case 9:
        DWRITE->RenderText(L"BossState : Attack_2_Left", fontrect, fontsize, fontstyle, Color(0, 0, 0, 1));
        break;
    case 10:
        DWRITE->RenderText(L"BossState : Attack_2_Right", fontrect, fontsize, fontstyle, Color(0, 0, 0, 1));
        break;
    case 11:
        DWRITE->RenderText(L"BossState : Spin_Attack", fontrect, fontsize, fontstyle, Color(0, 0, 0, 1));
        break;
    case 12:
        DWRITE->RenderText(L"BossState : Death_Start", fontrect, fontsize, fontstyle, Color(0, 0, 0, 1));
        break;
    case 13:
        DWRITE->RenderText(L"BossState : Death_End", fontrect, fontsize, fontstyle, Color(0, 0, 0, 1));
        break;
    case 14:
        DWRITE->RenderText(L"BossState : Parry_Impact", fontrect, fontsize, fontstyle, Color(0, 0, 0, 1));
        break;
    case 15:
        DWRITE->RenderText(L"BossState : Parry_Groggy", fontrect, fontsize, fontstyle, Color(0, 0, 0, 1));
        break;
    case 16:
        DWRITE->RenderText(L"BossState : Parry_Knock_Back", fontrect, fontsize, fontstyle, Color(0, 0, 0, 1));
        break;
    case 17:
        DWRITE->RenderText(L"BossState : Knock_Back_To_Idle", fontrect, fontsize, fontstyle, Color(0, 0, 0, 1));
        break;
    case 18:
        DWRITE->RenderText(L"BossState : Groggy_To_Idle", fontrect, fontsize, fontstyle, Color(0, 0, 0, 1));
        break;
    }

    DWRITE->RenderText(L"Interval  : " + to_wstring(boss_trail->interval), RECT{ 10,55,1000,1000 }, fontsize, fontstyle, Color(0, 0, 0, 1));
    DWRITE->RenderText(L"Trailsize : " + to_wstring(boss_trail->maxTrail), RECT{ 10,100,1000,1000 }, fontsize, fontstyle, Color(0, 0, 0, 1));

}
