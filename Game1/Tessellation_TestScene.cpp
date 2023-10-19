#include "stdafx.h"

Tessellation_TestScene::Tessellation_TestScene()
{

    cam = Camera::Create();
    cam->LoadFile("Cam.xml");

    Camera::main = cam;



    shader = new Shader();
    shader->LoadFile("TessTri.hlsl");
    shader->LoadTessellation();

    int vertexCount = 3;
    VertexP* vertices = new VertexP[vertexCount];
    vertices[0] = Vector3(0, 0.9, 0);
    vertices[1] = Vector3(0.9, -0.9, 0);
    vertices[2] = Vector3(-0.9, -0.9, 0);

    int indexCount = 3;
    UINT* indices = new UINT[indexCount];
    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;

    mesh = new Mesh(vertices, vertexCount, indices, indexCount, VertexType::P);
    mesh->primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST;

    edgeBufferDesc.edges.x = 1;
    edgeBufferDesc.edges.y = 1;
    edgeBufferDesc.edges.z = 1;
    edgeBufferDesc.inside = 1;

    {
        D3D11_BUFFER_DESC desc = { 0 };
        desc.ByteWidth = sizeof(EdgeBuffer);
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//상수버퍼
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;
        HRESULT hr = D3D->GetDevice()->CreateBuffer(&desc, NULL, &edgeBuffer);
        assert(SUCCEEDED(hr));

    }

}

Tessellation_TestScene::~Tessellation_TestScene()
{
    cam->Release();
}

void Tessellation_TestScene::Init()
{

}

void Tessellation_TestScene::Release()
{

}


void Tessellation_TestScene::Update()
{
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
    ImGui::End();

}

void Tessellation_TestScene::LateUpdate()
{
    ImGui::DragFloat("Edge0", &edgeBufferDesc.edges.x);
    ImGui::DragFloat("Edge1", &edgeBufferDesc.edges.y);
    ImGui::DragFloat("Edge2", &edgeBufferDesc.edges.z);
    ImGui::DragFloat("Inside0", &edgeBufferDesc.inside);
}

void Tessellation_TestScene::PreRender()
{


}

void Tessellation_TestScene::Render()
{
    LIGHT->Set();
    Camera::main->Set();
    RASTER->Set(D3D11_CULL_NONE, D3D11_FILL_WIREFRAME);
    {
        //상수버퍼
        D3D11_MAPPED_SUBRESOURCE mappedResource;
        D3D->GetDC()->Map(edgeBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
        memcpy_s(mappedResource.pData, sizeof(EdgeBuffer), &edgeBufferDesc, sizeof(EdgeBuffer));
        D3D->GetDC()->Unmap(edgeBuffer, 0);
        D3D->GetDC()->HSSetConstantBuffers(10, 1, &edgeBuffer);
    }
    shader->Set();
    mesh->Set();
    D3D->GetDC()->Draw(3, 0);
}

void Tessellation_TestScene::ResizeScreen()
{
    cam->width = App.GetWidth();
    cam->height = App.GetHeight();
    cam->viewport.width = App.GetWidth();
    cam->viewport.height = App.GetHeight();
}
