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

    post = new PostEffect(); // 씬전환시 페이드인 페이드아웃 사용할 포스트이펙트
   
    /*
    씬 넘버링 (추가하실경우 꼭 본인이사용할 씬과 씬의 용도를 적어주세요) - 신관희
    0. 로딩씬
    1. 씬2(메인게임)
    2. 보스스테이지씬
    3. 디퍼드 사용방법 테스트씬
    4. 공부전용씬
    5. 신관희 테스트씬 - 이펙트 / 트리 / 각종 파트별로 테스트해보고있습니다.
    6. 신관희 테셀레이션 테스트 전용 씬
    7. -
    8. -
    9. -
    10. -
    */

    int ChoiceScene = 5; // 변경하고자하는 씬의 번호로 바꾼뒤 실행해주세요!

    switch (ChoiceScene)
    {
    case 0:
    {
        SCENE->AddScene("LOADING_SCENE", new LoadingScene());
        SCENE->ChangeScene("LOADING_SCENE")->Init();
    }
    break;
    case 1:
    {
        SCENE->AddScene("MAIN_SCENE", new Scene2());
        SCENE->ChangeScene("MAIN_SCENE")->Init();
    }
    break;
    case 2:
    {
        SCENE->AddScene("BOSS_SCENE", new BossScene());
        SCENE->ChangeScene("BOSS_SCENE")->Init();
    }
    break;
    case 3:
    {
        SCENE->AddScene("DEFFERED_SCENE", new DeferredTestScene());
        SCENE->ChangeScene("DEFFERED_SCENE")->Init();
    }
    break;
    case 4:
    {
        SCENE->AddScene("STUDY_SCENE", new StudyScene());
        SCENE->ChangeScene("STUDY_SCENE")->Init();
    }
    break;
    case 5:
    {
        SCENE->AddScene("SKH_TEST_SCENE", new SKH_TestScene());
        SCENE->ChangeScene("SKH_TEST_SCENE")->Init();
    }
    break;
    case 6:
    {
        SCENE->AddScene("Tessellation_TestScene", new Tessellation_TestScene());
        SCENE->ChangeScene("Tessellation_TestScene")->Init();
    }
        break;
    case 7:
        break;
    case 8:
        break;
    case 9:
        break;
    case 10:
        break;
    }
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