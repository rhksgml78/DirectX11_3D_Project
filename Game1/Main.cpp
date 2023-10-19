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

    post = new PostEffect(); // ����ȯ�� ���̵��� ���̵�ƿ� ����� ����Ʈ����Ʈ
   
    /*
    �� �ѹ��� (�߰��Ͻǰ�� �� �����̻���� ���� ���� �뵵�� �����ּ���) - �Ű���
    0. �ε���
    1. ��2(���ΰ���)
    2. ��������������
    3. ���۵� ����� �׽�Ʈ��
    4. ���������
    5. �Ű��� �׽�Ʈ�� - ����Ʈ / Ʈ�� / ���� ��Ʈ���� �׽�Ʈ�غ����ֽ��ϴ�.
    6. �Ű��� �׼����̼� �׽�Ʈ ���� ��
    7. -
    8. -
    9. -
    10. -
    */

    int ChoiceScene = 5; // �����ϰ����ϴ� ���� ��ȣ�� �ٲ۵� �������ּ���!

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