#include "stdafx.h"

string initObject;
int     initCount;

void Func22()
{
    SCENE->AddScene("SC2", new Scene2());
    SCENE->GetScene("SC2")->Init();

    //SCENE->ChangeScene("SC2",1.0f)->Init();
}


LoadingScene::LoadingScene()
{
}

LoadingScene::~LoadingScene()
{
}

void LoadingScene::Init()
{
    ui = UI::Create();
    ui->LoadFile("Loading.xml");

    maxint = 4; // Scene2에 있는 init할 객체들의 수
    

    cam = Camera::Create();
    cam->LoadFile("Cam.xml");
    Camera::main = cam;

    Camera::main->width = App.GetWidth();
    Camera::main->height = App.GetHeight();
    Camera::main->viewport.width = App.GetWidth();
    Camera::main->viewport.height = App.GetHeight();

    //별도 스레드 동작
    t1 = new thread(Func22);

  
}

void LoadingScene::Release()
{
}

void LoadingScene::Update()
{
    post->RenderDetail();

    ImGui::Begin("Hierarchy");
    ui->RenderHierarchy();
    ImGui::End();

    // Scene2->Init()에서 추가하는 객체만큼 나누는 수 증가시키기
    ui->Find("bar")->scale.x = (float)initCount/ maxint;

    ui->Update();
    post->Update();
    Camera::main->Update();
    Camera::ControlMainCam();
}

void LoadingScene::LateUpdate()
{
    if (initCount >= maxint and
        SCENE->GetScene("LOADING")->state != SceneState::FADEOUT)
    {
        SCENE->ChangeScene("SC2", 2.0f);
    }

    if (SCENE->GetScene("LOADING")->state == SceneState::FADEOUT)
    {
        post->psp.color.x -= 0.5/2.0f * DELTA;
        post->psp.color.y -= 0.5/2.0f * DELTA;
        post->psp.color.z -= 0.5/2.0f * DELTA;
        //psp.radius -= 600.0f * DELTA;
    }

    ResizeScreen();
}

void LoadingScene::Render()
{
    wstring str = Util::ToWString(initObject) + to_wstring(initCount);
    
    DWRITE->RenderText(str, RECT{0, 0, 800, 800},40,L"궁서체",Color(1,0,0,1));

    post->Render();
   

}

void LoadingScene::PreRender()
{
    LIGHT->Set();
    Camera::main->Set();
    post->SetTarget();
    ui->Render();
}

void LoadingScene::ResizeScreen()
{
    Camera::main->width = App.GetWidth();
    Camera::main->height = App.GetHeight();
    Camera::main->viewport.width = App.GetWidth();
    Camera::main->viewport.height = App.GetHeight();
}
