#include "stdafx.h"

Scene1::Scene1()
{

    cam = Camera::Create();
    Camera::main = cam;



    sky = Sky::Create();

    map = Terrain::Create();
    map->LoadFile("Terrain_Sample.xml");
    map->CreateStructuredBuffer();

    cube = new Environment();
    RESOURCE->shaders.Load("0.SkyCubeMap.hlsl")->LoadGeometry();
    RESOURCE->shaders.Load("5.CubeMap.hlsl")->LoadGeometry();

    //cam->Update();
    /*sky->Update();
    map->Update();*/

    //미리 그려놓기 (6면체에 렌더링)
    //cube->SetTarget();
    //LIGHT->Set();
    ////렌더링
    //sky->Render(RESOURCE->shaders.Load("0.SkyCubeMap.hlsl"));
    //map->Render(RESOURCE->shaders.Load("5.CubeMap.hlsl"));

    //D3D->Present();


}

Scene1::~Scene1()
{


    cam->Release();
    sky->Release();
    map->Release();
}

void Scene1::Init()
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

void Scene1::Release()
{

}


void Scene1::Update()
{

    cube->RenderDetail();
    Camera::main->width = App.GetWidth();
    Camera::main->height = App.GetHeight();
    Camera::main->viewport.width = App.GetWidth();
    Camera::main->viewport.height = App.GetHeight();

    Camera::main->Update();
    Camera::ControlMainCam();

    LIGHT->RenderDetail();
    ImGui::Text("FPS: %d", TIMER->GetFramePerSecond());

    ImGui::Begin("Hierarchy");
    cam->RenderHierarchy();
    sky->RenderHierarchy();
    map->RenderHierarchy();
    ImGui::End();


    cam->Update();
    sky->Update();
    map->Update();

}

void Scene1::LateUpdate()
{

}

void Scene1::PreRender()
{
    static int count = 0;

    if (count < 2)
    {
        cube->SetTarget(Color(0, 0, 0, 1));
        cube->position = map->Find("None")->GetWorldPos();
        LIGHT->Set();
        sky->Render(RESOURCE->shaders.Load("0.SkyCubeMap.hlsl"));
        map->Render(RESOURCE->shaders.Load("5.CubeMap.hlsl"));
        count++;
    }


}

void Scene1::Render()
{
    LIGHT->Set();
    Camera::main->Set();

    cam->Render();
    sky->Render();

    cube->SetRGBTexture(4);


    map->Render();


}

void Scene1::ResizeScreen()
{
    cam->width = App.GetWidth();
    cam->height = App.GetHeight();
    cam->viewport.width = App.GetWidth();
    cam->viewport.height = App.GetHeight();


}
