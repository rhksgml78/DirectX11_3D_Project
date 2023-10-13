#include "stdafx.h"

DeferredTestScene::DeferredTestScene()
{
    cam = Camera::Create();
    cam->LoadFile("Cam.xml");

    Camera::main = cam;

    /*RESOURCE->shaders.Load("0.SkyCubeMap.hlsl")->LoadGeometry();
    RESOURCE->shaders.Load("4.CubeMap.hlsl")->LoadGeometry();
    RESOURCE->shaders.Load("5.CubeMap.hlsl")->LoadGeometry();*/

    player = Actor::Create();
    player->LoadFile("BossMonsterSE.xml");
    for (int i = 0; i < 10; i++)
    {
        playerClone[i] = Actor::Create(player);
        playerClone[i]->root->MoveWorldPos(Vector3(20+(i * 20), 0, 0));
        playerClone[i]->root->anim->ChangeAnimation(AnimationState::LOOP, 1);
    }

    player->root->anim->ChangeAnimation(AnimationState::LOOP, 1);

    deferred = new Deferred;

    post = Actor::Create();
    post->LoadFile("PostEffect.xml");
    post->shader->LoadFile("6.DeferredRender.hlsl");
}

DeferredTestScene::~DeferredTestScene()
{
    cam->Release();
}

void DeferredTestScene::Init()
{

}

void DeferredTestScene::Release()
{

}


void DeferredTestScene::Update()
{
    Camera::main->width = App.GetWidth();
    Camera::main->height = App.GetHeight();
    Camera::main->viewport.width = App.GetWidth();
    Camera::main->viewport.height = App.GetHeight();

    Camera::main->Update();
    Camera::ControlMainCam();

    LIGHT->RenderDetail();
    ImGui::Text("FPS: %d", TIMER->GetFramePerSecond());
    ImGui::SliderFloat("Main_Cam_Speed", &Camera::main->mainCamSpeed, 0.0f, 500.0f);

    ImGui::Begin("Hierarchy");

    cam->RenderHierarchy();
    player->RenderHierarchy();
    for (int i = 0; i < 10; i++)
    {
        playerClone[i]->RenderHierarchy();
    }
    ImGui::End();


    cam->Update();
    player->Update();
    post->Update();
    for (int i = 0; i < 10; i++)
    {
        playerClone[i]->Update();
    }
    deferred->RenderDetail();
}

void DeferredTestScene::LateUpdate()
{

}

void DeferredTestScene::PreRender()
{
    LIGHT->Set();
    Camera::main->Set();
    deferred->SetTarget();

    player->Render(RESOURCE->shaders.Load("4.Deferred.hlsl"));
    for (int i = 0; i < 10; i++)
    {
        playerClone[i]->Render(RESOURCE->shaders.Load("4.Deferred.hlsl"));
    }
}

void DeferredTestScene::Render()
{
    /*LIGHT->Set();
    Camera::main->Set();

    for (int i = 0; i < 10; i++)
    {
        playerClone[i]->Render();
    }
    player->Render();*/
    deferred->SetRGBTexture(10);
    post->Render();
}

void DeferredTestScene::ResizeScreen()
{
    cam->width = App.GetWidth();
    cam->height = App.GetHeight();
    cam->viewport.width = App.GetWidth();
    cam->viewport.height = App.GetHeight();

    deferred->ResizeScreen(App.GetWidth(), App.GetHeight());
}
