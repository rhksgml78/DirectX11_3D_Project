#include "stdafx.h"

StudyScene::StudyScene()
{
}

StudyScene::~StudyScene()
{
}

void StudyScene::Init()
{

    cam = Camera::Create();
    cam->LoadFile("Cam.xml");
    Camera::main = cam;

    /*cube->SetTarget(Color(0, 0, 0, 1));

    sky->Update();
    map->Update();
    cube->position = map->Find("None")->GetWorldPos();
    LIGHT->Set();
    sky->Render(RESOURCE->shaders.Load("0.SkyCubeMap.hlsl"));
    map->Render(RESOURCE->shaders.Load("5.CubeMap.hlsl"));
    D3D->Present();*/

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
    player->LoadFile("BossMonsterSE.xml");
    player->anim->ChangeAnimation(AnimationState::LOOP, 2, 0.1f);

    inven = Inven::Create();
    inven->LoadFile("Inven_Study.xml"); // ���͸� ����
    inven->LoadInven(); // �κ��� ����



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
    envi2 = new Environment(1024, 1024); // ���� ���� ȿ���� ���� ȯ�水ü2
    //23.10.12 ����
    //RESOURCE->shaders.Load("0.SkyCubeMap.hlsl")->LoadGeometry();
    //RESOURCE->shaders.Load("4.CubeMap.hlsl")->LoadGeometry();
    //RESOURCE->shaders.Load("5.CubeMap.hlsl")->LoadGeometry();
    //�� 3���� ���̴���� X ���� �߰��� 4�� 5�� ���͸� ���̴� ������� ����
    RESOURCE->shaders.Load("4.WaterMap.hlsl")->LoadGeometry();
    RESOURCE->shaders.Load("5.WaterMap.hlsl")->LoadGeometry();


    {
        // ��� ���� ���� 23.10.11 �߰�
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
    {
        // ����ȿ��(��������)�� �����ؾ��ϴ� ���� ��� ����! 23.10.12 �߰�
        D3D11_BUFFER_DESC desc = { 0 };
        desc.ByteWidth = sizeof(Vector4); // ����� �ܼ��� Vector4�����̴�.
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//�������
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;
        HRESULT hr = D3D->GetDevice()->CreateBuffer(&desc, NULL, &planeBuffer);
        assert(SUCCEEDED(hr));

    }
#pragma endregion

#pragma region Sample

#pragma endregion

}

void StudyScene::Release()
{
#pragma region Water_Effect
    SafeDelete(envi2);
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
    sky->Release();
    map->Release();
}


void StudyScene::Update()
{
    // ī�޶� ����
    Camera::main->width = App.GetWidth();
    Camera::main->height = App.GetHeight();
    Camera::main->viewport.width = App.GetWidth();
    Camera::main->viewport.height = App.GetHeight();

    Camera::main->Update();
    Camera::ControlMainCam();
   
    // ���� ������ Ȯ��
    LIGHT->RenderDetail();
    //cube->RenderDetail();
    ImGui::Text("FPS: %d", TIMER->GetFramePerSecond());
    ImGui::SliderFloat(" App.deltaScale", &App.deltaScale, 0.0f, 10.0f);

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
    inven->RenderHierarchy();
    shadow->RenderDetail();
    ImGui::End();

    sky->Update();
    map->Update();
    player->Update();
    inven->Update();
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
    shadow->SetRGBTexture(7);
    shadow->SetTarget(player->GetWorldPos());
    player->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));

#pragma region Water_Effect
    {
        // �������Ŀ� ����� ��� pl 23.10.12 �߰�
        Plane pl(water->GetWorldPos(), water->GetUp()); // ������
        {
            plane = pl;

            //�������
            D3D11_MAPPED_SUBRESOURCE mappedResource;
            D3D->GetDC()->Map(planeBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
            memcpy_s(mappedResource.pData, sizeof(Vector4), &plane, sizeof(Vector4));
            D3D->GetDC()->Unmap(planeBuffer, 0);
            D3D->GetDC()->VSSetConstantBuffers(6, 1, &planeBuffer);
        }

        // �ݻ���� �ؽ�ó 23.10.11 �߰�
        Vector3 Dir = water->GetWorldPos() - Camera::main->GetWorldPos();
        float Distance = Dir.Length();
        Dir.Normalize();
        Vector3 reflect = Vector3::Reflect(Dir, water->GetUp());
        envi->position = (water->GetWorldPos() - reflect * Distance);
        envi->SetTarget(Color(0, 0, 0, 1));

        // ���������� ���, �÷��̾�, �ʱ��� �ݻ翡 �־��µ� �ʿ������!
        //sky->Render(RESOURCE->shaders.Load("0.SkyCubeMap.hlsl")); // ������ �ݻ��Ű���� �ּ�����
        //player->Render(RESOURCE->shaders.Load("4.CubeMap.hlsl"));
        //map->Render(RESOURCE->shaders.Load("5.CubeMap.hlsl"));
        
        // ���� �߰��� ���̴��� �÷��̾�� �ͷ��ο�������ش�!
        player->Render(RESOURCE->shaders.Load("4.WaterMap.hlsl"));
        map->Render(RESOURCE->shaders.Load("5.WaterMap.hlsl"));
    }

    {
        // �������Ŀ� ����� ��� pl 23.10.12 �߰�
        //                              Y�� 1���������� > ǥ��ƴ��������
        Plane pl(water->GetWorldPos() + Vector3(0, 1, 0), -water->GetUp()); // �Ʒ�����
        {
            //plane = Vector4(0, -1, 0, water->GetWorldPos().y);
            plane = pl;
            //�������
            D3D11_MAPPED_SUBRESOURCE mappedResource;
            D3D->GetDC()->Map(planeBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
            memcpy_s(mappedResource.pData, sizeof(Vector4), &plane, sizeof(Vector4));
            D3D->GetDC()->Unmap(planeBuffer, 0);
            D3D->GetDC()->VSSetConstantBuffers(6, 1, &planeBuffer);
        }
        // ����ȿ���� ���ؼ� ���� ���� ���� ����  23.10.11 �߰�
        envi2->position = Camera::main->GetWorldPos(); // ���������ġ
        envi2->SetTarget(Color(0, 0, 0, 1));

        // ���������� ���, �÷��̾�, �ʱ��� �ݻ翡 �־��µ� �ʿ������!
        //sky->Render(RESOURCE->shaders.Load("0.SkyCubeMap.hlsl"));
        //player->Render(RESOURCE->shaders.Load("4.CubeMap.hlsl"));
        //map->Render(RESOURCE->shaders.Load("5.CubeMap.hlsl"));

        // ���� �߰��� ���̴��� �÷��̾�� �ͷ��ο�������ش�!
        player->Render(RESOURCE->shaders.Load("4.WaterMap.hlsl"));
        map->Render(RESOURCE->shaders.Load("5.WaterMap.hlsl"));
    }
#pragma endregion
}

void StudyScene::Render()
{
    Camera::main->Set();
    LIGHT->Set();

    sky->Render();
    player->Render();
    map->Render();
    //cube->SetRGBTexture(4);
    //inven->Render();
    // �ν��Ͻ�
#pragma region instuncing
    //actorInstance->Render();
#pragma endregion

#pragma region Water_Effect
    envi->SetRGBTexture(4); // ���̴����ҽ�, ���÷������� ���Թ�ȣ
    envi2->SetRGBTexture(5); // ���̴����ҽ�, ���÷������� ���Թ�ȣ
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
    Camera::main->width = App.GetWidth();
    Camera::main->height = App.GetHeight();
    Camera::main->viewport.width = App.GetWidth();
    Camera::main->viewport.height = App.GetHeight();
}
