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
    map->LoadFile("DJ_Terrain.xml");
    map->CreateStructuredBuffer();
    map->material->shadow = 0.5f;

    shadow = new Shadow();
    //ps쉐이더가 다른쉐이더
    RESOURCE->shaders.Load("4.ShadowMap.hlsl");
    RESOURCE->shaders.Load("2.ShadowMap.hlsl");

    player = Actor::Create();
    player->LoadFile("BossMonsterSE.xml");
    player->anim->ChangeAnimation(AnimationState::LOOP, 1, 0.1f);

    inven = Inven::Create();
    inven->LoadFile("Inven_Study.xml"); // 액터를 구성
    inven->LoadInven(); // 인벤을 구성



    // 루트모션으로 만들기
    player->Find("mixamorig:Hips")->rootMotion = true;

    //인스턴싱 수업
#pragma region instuncing

    //// 인스턴싱
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

    ////이 액터안에  actor[i][j][k] 트랜스폼을 옮기기
    //actorInstance = Actor::Create("Instance_Test_Actor");
    //actorInstance->LoadFile("SphereInstance.xml");
    //actorInstance->name = "Instance_Test_Actor";

    //// 매트릭스 임시 객체를 생성한다
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
    //            count++; // 인스턴스 갯수세기
    //        }
    //    }
    //}
    //// 액터인스턴스객체의 메쉬를 통하여 인스턴스버퍼를
    //// 매트릭스임시객체의 크기만큼 & 인스턴스 갯수만큼 생성한다.
    //actorInstance->mesh->CreateInstanceBuffer(temp, count);
#pragma endregion

    //물효과 수업
#pragma region Water_Effect
    water = Actor::Create();
    water->LoadFile("water.xml");

    envi = new Environment(1024, 1024); // 환경이미지 크기늘리기
    envi2 = new Environment(1024, 1024); // 수면 굴절 효과를 위한 환경객체2
    //23.10.12 수정
    //RESOURCE->shaders.Load("0.SkyCubeMap.hlsl")->LoadGeometry();
    //RESOURCE->shaders.Load("4.CubeMap.hlsl")->LoadGeometry();
    //RESOURCE->shaders.Load("5.CubeMap.hlsl")->LoadGeometry();
    //위 3개의 쉐이더사용 X 새로 추가한 4번 5번 워터맵 쉐이더 사용으로 변경
    RESOURCE->shaders.Load("4.WaterMap.hlsl")->LoadGeometry();
    RESOURCE->shaders.Load("5.WaterMap.hlsl")->LoadGeometry();


    {
        // 상수 버퍼 생성 23.10.11 추가
        D3D11_BUFFER_DESC desc = { 0 };
        desc.ByteWidth = sizeof(WaterBuffer);
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//상수버퍼
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;
        HRESULT hr = D3D->GetDevice()->CreateBuffer(&desc, NULL, &waterBuffer);
        assert(SUCCEEDED(hr));
    }
    {
        // 굴절효과(평면방정식)을 응용해야하는 버퍼 평면 버퍼! 23.10.12 추가
        D3D11_BUFFER_DESC desc = { 0 };
        desc.ByteWidth = sizeof(Vector4); // 사이즈가 단순히 Vector4형태이다.
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//상수버퍼
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
    // 카메라 설정
    Camera::main->width = App.GetWidth();
    Camera::main->height = App.GetHeight();
    Camera::main->viewport.width = App.GetWidth();
    Camera::main->viewport.height = App.GetHeight();

    Camera::main->Update();
    Camera::ControlMainCam();
   
    // 조명 디테일 확인
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
    //인스턴싱
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
    //환경 맵핑 설정하기 !
    //cube->position = sphere->GetWorldPos();
    //cube->SetTarget(Color(0, 0, 0, 1));
    //sky->Render(RESOURCE->shaders.Load("0.SkyCubeMap.hlsl"));
    //map->Render(RESOURCE->shaders.Load("5.CubeMap.hlsl"));
    //player->Render(RESOURCE->shaders.Load("4.CubeMap.hlsl"));


#pragma region Water_Effect
    {
        // 평면방정식에 사용할 평면 pl 23.10.12 추가
        Plane pl(water->GetWorldPos(), water->GetUp()); // 윗방향
        {
            plane = pl;

            //상수버퍼
            D3D11_MAPPED_SUBRESOURCE mappedResource;
            D3D->GetDC()->Map(planeBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
            memcpy_s(mappedResource.pData, sizeof(Vector4), &plane, sizeof(Vector4));
            D3D->GetDC()->Unmap(planeBuffer, 0);
            D3D->GetDC()->VSSetConstantBuffers(6, 1, &planeBuffer);
        }

        // 반사맵핑 텍스처 23.10.11 추가
        Vector3 Dir = water->GetWorldPos() - Camera::main->GetWorldPos();
        float Distance = Dir.Length();
        Dir.Normalize();
        Vector3 reflect = Vector3::Reflect(Dir, water->GetUp());
        envi->position = (water->GetWorldPos() - reflect * Distance);
        envi->SetTarget(Color(0, 0, 0, 1));

        // 수정전에는 배경, 플레이어, 맵까지 반사에 넣었는데 필요없어짐!
        //sky->Render(RESOURCE->shaders.Load("0.SkyCubeMap.hlsl")); // 배경까지 반사시키려면 주석해제
        //player->Render(RESOURCE->shaders.Load("4.CubeMap.hlsl"));
        //map->Render(RESOURCE->shaders.Load("5.CubeMap.hlsl"));
        
        // 새로 추가한 쉐이더를 플레이어와 터레인에만들어준다!
        player->Render(RESOURCE->shaders.Load("4.WaterMap.hlsl"));
        map->Render(RESOURCE->shaders.Load("5.WaterMap.hlsl"));
    }

    {
        // 평면방정식에 사용할 평면 pl 23.10.12 추가
        //                              Y축 1더해준이유 > 표면틈새때문에
        Plane pl(water->GetWorldPos() + Vector3(0, 1, 0), -water->GetUp()); // 아랫방향
        {
            //plane = Vector4(0, -1, 0, water->GetWorldPos().y);
            plane = pl;
            //상수버퍼
            D3D11_MAPPED_SUBRESOURCE mappedResource;
            D3D->GetDC()->Map(planeBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
            memcpy_s(mappedResource.pData, sizeof(Vector4), &plane, sizeof(Vector4));
            D3D->GetDC()->Unmap(planeBuffer, 0);
            D3D->GetDC()->VSSetConstantBuffers(6, 1, &planeBuffer);
        }
        // 굴절효과를 위해서 별도 렌더 구간 마련  23.10.11 추가
        envi2->position = Camera::main->GetWorldPos(); // 현재시점위치
        envi2->SetTarget(Color(0, 0, 0, 1));

        // 수정전에는 배경, 플레이어, 맵까지 반사에 넣었는데 필요없어짐!
        //sky->Render(RESOURCE->shaders.Load("0.SkyCubeMap.hlsl"));
        //player->Render(RESOURCE->shaders.Load("4.CubeMap.hlsl"));
        //map->Render(RESOURCE->shaders.Load("5.CubeMap.hlsl"));

        // 새로 추가한 쉐이더를 플레이어와 터레인에만들어준다!
        player->Render(RESOURCE->shaders.Load("4.WaterMap.hlsl"));
        map->Render(RESOURCE->shaders.Load("5.WaterMap.hlsl"));
    }
#pragma endregion

    // 그림자 매핑 설정하기!
    shadow->SetTarget(player->GetWorldPos());
    player->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
    map->Render(RESOURCE->shaders.Load("5.ShadowMap.hlsl"));
    water->Render(RESOURCE->shaders.Load("4.ShadowMap.hlsl"));
}

void StudyScene::Render()
{
    Camera::main->Set();
    LIGHT->Set();
    shadow->SetRGBTexture(7);

    sky->Render();
    player->Render();
    map->Render();
    //cube->SetRGBTexture(4);
    //inven->Render();
    // 인스턴싱
#pragma region instuncing
    //actorInstance->Render();
#pragma endregion

#pragma region Water_Effect
    envi->SetRGBTexture(4); // 쉐이더리소스, 샘플러설정용 슬롯번호
    envi2->SetRGBTexture(5); // 쉐이더리소스, 샘플러설정용 슬롯번호
    {
        waterBufferDesc.time = TIMER->GetWorldTime() * waterspeed;
        waterBufferDesc.dir = waterdir;
        waterBufferDesc.scale = uvscale;
        waterBufferDesc.refractscalar = refractscalar;
        waterBufferDesc.padding.x = rttpad.x;
        waterBufferDesc.padding.y = rttpad.y;
        waterBufferDesc.padding.z = rttpad.z;
        //상수버퍼
        D3D11_MAPPED_SUBRESOURCE mappedResource;
        D3D->GetDC()->Map(waterBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
        memcpy_s(mappedResource.pData, sizeof(WaterBuffer), &waterBufferDesc, sizeof(WaterBuffer));
        D3D->GetDC()->Unmap(waterBuffer, 0);
        D3D->GetDC()->PSSetConstantBuffers(10, 1, &waterBuffer);
    }
    water->Render();
#pragma endregion
    if (ImGui::Button("ObjectAxis_ON/OFF"))
    {
        axistnf = !axistnf;
        WIN->SetAxis(axistnf);
    }
    
    ImGui::SliderFloat("MainCamSpeed",&Camera::mainCamSpeed,0.0f,100.0f);
    ImGui::SliderFloat("Refract",&refractscalar,0.0f,10.0f);
    ImGui::SliderFloat("WaterSpeed",&waterspeed,0.0f,5.0f);
    ImGui::SliderFloat("WaterDir_X(-1~1)",&waterdir.x,-1.0f,1.0f);
    ImGui::SliderFloat("WaterDir_Z(-1~1)",&waterdir.y,-1.0f,1.0f);
    ImGui::SliderFloat("WaterUV",&uvscale,0.0f,10.0f);
    
    if (INPUT->KeyDown(VK_NUMPAD1))
    {
        rttpad = { 1,0,0 };
    }
    if (INPUT->KeyDown(VK_NUMPAD2))
    {
        rttpad = { 0,1,0 };
    }
    if (INPUT->KeyDown(VK_NUMPAD3))
    {
        rttpad = { 0,0,1 };
    }
    

    if (rttpad.x == 1)
    {
        DWRITE->RenderText(L"Only RTT_1 Texture ",
            RECT{ 200,125,static_cast<LONG>(App.GetWidth()),static_cast<LONG>(App.GetHeight()) }, 25.0f
            , L"던파 연단된 칼날");
    }
    if (rttpad.y == 1)
    {
        DWRITE->RenderText(L"Only RTT_2 Texture ",
            RECT{ 200,125,static_cast<LONG>(App.GetWidth()),static_cast<LONG>(App.GetHeight()) }, 25.0f
            , L"던파 연단된 칼날");
    }
    if (rttpad.z == 1)
    {
        DWRITE->RenderText(L"RTT_1 + RTT_2 Texture ",
            RECT{ 200,125,static_cast<LONG>(App.GetWidth()),static_cast<LONG>(App.GetHeight()) }, 25.0f
            , L"던파 연단된 칼날");
    }

    
    
    //DWRITE->RenderText(L"환경반사 (0~1) : " + to_wstring(water->material->environment),
    //    RECT{ 200,125,static_cast<LONG>(App.GetWidth()),static_cast<LONG>(App.GetHeight()) }, 25.0f
    //    , L"던파 연단된 칼날");
    //DWRITE->RenderText(L"속도 : " + to_wstring(waterspeed),
    //    RECT{ 200,155,static_cast<LONG>(App.GetWidth()),static_cast<LONG>(App.GetHeight()) }, 25.0f
    //    , L"던파 연단된 칼날");
    //DWRITE->RenderText(L"방향 : [X]" + to_wstring(waterdir.x)+L" [Z]" + to_wstring(waterdir.y),
    //    RECT{ 200,185,static_cast<LONG>(App.GetWidth()),static_cast<LONG>(App.GetHeight()) }, 25.0f
    //    , L"던파 연단된 칼날");
    //DWRITE->RenderText(L"이미지 비율 : " + to_wstring((float)uvscale),
    //    RECT{ 200,215,static_cast<LONG>(App.GetWidth()),static_cast<LONG>(App.GetHeight()) }, 25.0f
    //    , L"던파 연단된 칼날");
}

void StudyScene::ResizeScreen()
{
    Camera::main->width = App.GetWidth();
    Camera::main->height = App.GetHeight();
    Camera::main->viewport.width = App.GetWidth();
    Camera::main->viewport.height = App.GetHeight();
}
