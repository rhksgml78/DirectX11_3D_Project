#include "stdafx.h"

StudyScene::StudyScene()
{
    
    cam = Camera::Create();
    cam->LoadFile("Cam.xml");
    Camera::main = cam;

    sky = Sky::Create();
    sky->texCube->LoadFile("sunsetcube1024.dds");

    map = Terrain::Create();
    map->LoadFile("Terrain_Study.xml");
    map->CreateStructuredBuffer();
    map->material->shadow = 0.5f;

    shadow = new Shadow();
    //ps���̴��� �ٸ����̴�
    RESOURCE->shaders.Load("4.ShadowMap.hlsl");
    RESOURCE->shaders.Load("2.ShadowMap.hlsl");

    player = Actor::Create();
    player->LoadFile("Player_Study.xml");
    player->anim->ChangeAnimation(AnimationState::LOOP, 0, 0.1f);

    inven = Inven::Create();
    inven->LoadFile("Inven_Study.xml"); // ���͸� ����
    inven->LoadInven(); // �κ��� ����

    sphere = Actor::Create();
    sphere->LoadFile("Sphere.xml");

    // ��Ʈ������� �����
    player->Find("mixamorig:Hips")->rootMotion = true;

    //�ν��Ͻ� ����
#pragma region instuncing
    //// �ν��Ͻ�
    //for (int i = 0; i < SIZE; i++)
    //{
    //    for (int j = 0; j < SIZE; j++)
    //    {
    //        for (int k = 0; k < SIZE; k++)
    //        {
    //            actor2[i][j][k] = Actor::Create();
    //            actor2[i][j][k]->LoadFile("Sphere.xml");
    //            actor2[i][j][k]->SetWorldPos(Vector3(i * 3, j * 3, k * 3));
    //        }
    //    }
    //}

    ////�� ���;ȿ�  actor[i][j][k] Ʈ�������� �ű��
    //actorInstance = Actor::Create("Instance_Test_Actor");
    //actorInstance->LoadFile("SphereInstance.xml");
    //actorInstance->name = "Instance_Test_Actor";

    //// ��Ʈ���� �ӽ� ��ü�� �����Ѵ�
    //Matrix* temp = new Matrix[SIZE * SIZE * SIZE];
    //UINT count = 0;

    //for (int i = 0; i < SIZE; i++)
    //{
    //    for (int j = 0; j < SIZE; j++)
    //    {
    //        for (int k = 0; k < SIZE; k++)
    //        {
    //            actor2[i][j][k]->Update();
    //            temp[count] = actor2[i][j][k]->W.Transpose();
    //            count++; // �ν��Ͻ� ��������
    //        }
    //    }
    //}
    //// �����ν��Ͻ���ü�� �޽��� ���Ͽ� �ν��Ͻ����۸�
    //// ��Ʈ�����ӽð�ü�� ũ�⸸ŭ & �ν��Ͻ� ������ŭ �����Ѵ�.
    //actorInstance->mesh->CreateInstanceBuffer(temp, count);
#pragma endregion

    //��ȿ�� ����
#pragma region Water_Effect
    water = Actor::Create();
    water->LoadFile("water.xml");

    envi = new Environment(1024, 1024); // ȯ���̹��� ũ��ø���
    RESOURCE->shaders.Load("0.SkyCubeMap.hlsl")->LoadGeometry();
    RESOURCE->shaders.Load("4.CubeMap.hlsl")->LoadGeometry();
    RESOURCE->shaders.Load("5.CubeMap.hlsl")->LoadGeometry();

    // ��� ���� ����
    {
        D3D11_BUFFER_DESC desc = { 0 };
        desc.ByteWidth = sizeof(WaterBuffer);
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//�������
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;
        HRESULT hr = D3D->GetDevice()->CreateBuffer(&desc, NULL, &waterBuffer);
        assert(SUCCEEDED(hr));
    }
#pragma endregion

#pragma region Water_Effect
#pragma endregion


}

StudyScene::~StudyScene()
{
#pragma region Water_Effect
    SafeDelete(envi);
    water->Release();
#pragma endregion

#pragma region instuncing
    //for (int i = 0; i < SIZE; i++)
    //{
    //    for (int j = 0; j < SIZE; j++)
    //    {
    //        for (int k = 0; k < SIZE; k++)
    //        {
    //            actor2[i][j][k]->Release();
    //        }
    //    }
    //}
    //actorInstance->Release();
#pragma endregion
    cam->Release();
    sky->Release();
    map->Release();
}

void StudyScene::Init()
{


    /*cube->SetTarget(Color(0, 0, 0, 1));

    sky->Update();
    map->Update();
    cube->position = map->Find("None")->GetWorldPos();
    LIGHT->Set();
    sky->Render(RESOURCE->shaders.Load("0.SkyCubeMap.hlsl"));
    map->Render(RESOURCE->shaders.Load("5.CubeMap.hlsl"));
    D3D->Present();*/
}

void StudyScene::Release()
{

}


void StudyScene::Update()
{

    //cube->RenderDetail();
    shadow->RenderDetail();

    Camera::main->width = App.GetWidth();
    Camera::main->height = App.GetHeight();
    Camera::main->viewport.width = App.GetWidth();
    Camera::main->viewport.height = App.GetHeight();

    Camera::main->Update();
    Camera::ControlMainCam();

    LIGHT->RenderDetail();
    ImGui::Text("FPS: %d", TIMER->GetFramePerSecond());

    ImGui::Begin("Hierarchy");

#pragma region instuncing
    //actorInstance->RenderHierarchy();
#pragma endregion
#pragma region Water_Effect
    water->RenderHierarchy();
#pragma endregion
    cam->RenderHierarchy();
    sky->RenderHierarchy();
    map->RenderHierarchy();
    player->RenderHierarchy();
    sphere->RenderHierarchy();
    inven->RenderHierarchy();
    ImGui::End();


    cam->Update();
    sky->Update();
    map->Update();
    player->Update();
    inven->Update();
    sphere->Update();
    //�ν��Ͻ�
#pragma region instuncing
    //actorInstance->Update();
#pragma endregion
#pragma region Water_Effect
    water->Update();
#pragma endregion
}

void StudyScene::LateUpdate()
{

}

void StudyScene::PreRender()
{
    Camera::main->Set();
    LIGHT->Set();

    //ȯ�� ���� �����ϱ� !
    //cube->position = sphere->GetWorldPos();
    //cube->SetTarget(Color(0, 0, 0, 1));
    //sky->Render(RESOURCE->shaders.Load("0.SkyCubeMap.hlsl"));
    //map->Render(RESOURCE->shaders.Load("5.CubeMap.hlsl"));
    //player->Render(RESOURCE->shaders.Load("4.CubeMap.hlsl"));

    // �׸��� ���� �����ϱ�!
    shadow->SetTarget(player->GetWorldPos());
    sphere->Render(RESOURCE->shaders.Load("2.ShadowMap.hlsl"));
    player->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));

#pragma region Water_Effect
    {
        // Render �����ܰ��� PreRender�ܰ迡�� envi�� ��ġ�� ����
        Vector3 Dir = water->GetWorldPos() - Camera::main->GetWorldPos();
        float Distance = Dir.Length();
        Dir.Normalize();
        Vector3 reflect = Vector3::Reflect(Dir, water->GetUp());
        envi->position = (water->GetWorldPos() - reflect * Distance);
    }
    envi->SetTarget(Color(0, 0, 0, 1));
    sky->Render(RESOURCE->shaders.Load("0.SkyCubeMap.hlsl"));
    player->Render(RESOURCE->shaders.Load("4.CubeMap.hlsl"));
    map->Render(RESOURCE->shaders.Load("5.CubeMap.hlsl"));
#pragma endregion
}

void StudyScene::Render()
{
    Camera::main->Set();
    LIGHT->Set();
    shadow->SetRGBTexture(7);

    cam->Render();
    sky->Render();
    player->Render();
    map->Render();
    //cube->SetRGBTexture(4);
    sphere->Render();
    //inven->Render();
    // �ν��Ͻ�
#pragma region instuncing
    //actorInstance->Render();
#pragma endregion
#pragma region Water_Effect
    envi->SetRGBTexture(4);
    {
        waterBufferDesc.time = TIMER->GetWorldTime() / 5.0f;
        waterBufferDesc.dir = Vector2(-1, -1);
        //�������
        D3D11_MAPPED_SUBRESOURCE mappedResource;
        D3D->GetDC()->Map(waterBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
        memcpy_s(mappedResource.pData, sizeof(WaterBuffer), &waterBufferDesc, sizeof(WaterBuffer));
        D3D->GetDC()->Unmap(waterBuffer, 0);
        D3D->GetDC()->PSSetConstantBuffers(10, 1, &waterBuffer);
    }
    water->Render();
#pragma endregion
    
    
    // ȭ�鿡�� ������ Ȯ���ϱ��
    DWRITE->RenderText(L"FRAME : " + to_wstring(TIMER->GetFramePerSecond()), RECT{ 5,5,500,500 });
}

void StudyScene::ResizeScreen()
{
    cam->width = App.GetWidth();
    cam->height = App.GetHeight();
    cam->viewport.width = App.GetWidth();
    cam->viewport.height = App.GetHeight();


}
