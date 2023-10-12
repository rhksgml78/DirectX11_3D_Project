#include "stdafx.h"
#include "Main.h"
#include "Scene2.h"
#include "LoadingScene.h"
#include "StudyScene.h"


PostEffect* post;


Main::Main()
{

}

Main::~Main()
{

}


void Main::Init()
{

    post = new PostEffect();
   
    //SCENE->AddScene("LOADING", new LoadingScene());
    //SCENE->ChangeScene("LOADING")->Init();

    /*SCENE->AddScene("Scene2", new Scene2());
    SCENE->ChangeScene("Scene2")->Init();*/

    //SCENE->AddScene("STUDY", new StudyScene());
    //SCENE->ChangeScene("STUDY")->Init();
    
    //SCENE->AddScene("SKH_TEST", new SKH_TestScene());
    //SCENE->ChangeScene("SKH_TEST")->Init();

    //// Å×½ºÆ®¾À
    SCENE->AddScene("BossScene", new BossScene());
    SCENE->ChangeScene("BossScene")->Init();
}

void Main::Release()
{
    delete post;

    SCENE->Release();
}


void Main::Update()
{
    SCENE->Update();
}

void Main::LateUpdate()
{
    SCENE->LateUpdate();
}
void Main::PreRender()
{
    SCENE->PreRender();
}
void Main::Render()
{
    SCENE->Render();
}

void Main::ResizeScreen()
{
    SCENE->ResizeScreen();
}

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE prevInstance, LPWSTR param, int command)
{
    App.SetAppName(L"Game1");
    App.SetInstance(instance);
    WIN->Create();
    D3D->Create();
    Inven::CreateStaticMember();
    Main* main = new Main();
    main->Init();
    int wParam = (int)WIN->Run(main);
    main->Release();
    Inven::DeleteStaticMember();
    SafeDelete(main);
    D3D->DeleteSingleton();
    WIN->DeleteSingleton();

    return wParam;
}