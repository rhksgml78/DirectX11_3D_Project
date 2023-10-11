#include "framework.h"

SlashTrail::SlashTrail()
{
    vertexType = VertexType::PT;
    primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    byteWidth = sizeof(VertexPT);


    shader = RESOURCE->shaders.Load("6.Trail.hlsl");
    isPlaying = false;
    time = 0.0f;
    interval = 0.1f;
    maxTrail = 10;

    material = make_shared<Material>();
    //material->LoadFile(".mtl");
}

SlashTrail::~SlashTrail()
{
}

void SlashTrail::Play()
{
    isPlaying = true;
    lastTop = Vector3();
    lastBottom = Vector3();
    time = 0.0f;
}

void SlashTrail::Stop()
{
    isPlaying = false;
    //�����ߴ� Ʈ������ ����
    vertices.clear();
    SafeRelease(vertexBuffer);
}

void SlashTrail::Update()
{
    if (not isPlaying) return;

    if (lastTop != Vector3() and lastBottom != Vector3())
    {
        if (TIMER->GetTick(time, interval))
        {
            vertices.push_back(VertexPT(lastTop, Vector2(0, 0)));
            vertices.push_back(VertexPT(lastBottom, Vector2(0, 0)));
            vertices.push_back(VertexPT(Top->GetWorldPos(), Vector2(0, 0)));

            vertices.push_back(VertexPT(lastBottom, Vector2(0, 0)));
            vertices.push_back(VertexPT(Bottom->GetWorldPos(), Vector2(0, 0)));
            vertices.push_back(VertexPT(Top->GetWorldPos(), Vector2(0, 0)));

            //���� �ִ� �簢�� ������ �Ѿ�����

            if (vertices.size() > maxTrail * 6)
            {
                vertices.erase(vertices.begin());
                vertices.erase(vertices.begin());
                vertices.erase(vertices.begin());
                vertices.erase(vertices.begin());
                vertices.erase(vertices.begin());
                vertices.erase(vertices.begin());
            }

            //�簢�� ������ŭ �ݺ�
            int count = vertices.size() / 6;

            for (int i = 0; i < count; i++)
            {
                vertices[i * 6 + 0].uv = Vector2((float)(count - i) / (float)count, 0);
                vertices[i * 6 + 1].uv = Vector2((float)(count - i) / (float)count, 1);
                vertices[i * 6 + 2].uv = Vector2((float)(count - i + 1) / (float)count, 0);
                vertices[i * 6 + 3].uv = Vector2((float)(count - i) / (float)count, 1);
                vertices[i * 6 + 4].uv = Vector2((float)(count - i + 1) / (float)count, 1);
                vertices[i * 6 + 5].uv = Vector2((float)(count - i + 1) / (float)count, 0);

            }

            //CreateVertexBuffer
            {
                D3D11_BUFFER_DESC desc;
                desc = { 0 };
                desc.Usage = D3D11_USAGE_DEFAULT;
                desc.ByteWidth = byteWidth * vertices.size();
                desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

                D3D11_SUBRESOURCE_DATA data = { 0 };
                data.pSysMem = &vertices[0];
                SafeRelease(vertexBuffer);
                HRESULT hr = D3D->GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
                assert(SUCCEEDED(hr));
            }

            lastTop = Top->GetWorldPos();
            lastBottom = Bottom->GetWorldPos();
        }
    }
    else
    {
        lastTop = Top->GetWorldPos();
        lastBottom = Bottom->GetWorldPos();
    }

}

void SlashTrail::Render()
{
    if (not isPlaying) return;

    if (vertices.size() < 1) return;

    UINT offset = 0;

    D3D->GetDC()->IASetVertexBuffers(0,
        1,
        &vertexBuffer,
        &byteWidth,
        &offset);
    D3D->GetDC()->IASetPrimitiveTopology
    (primitiveTopology);
    material->Set();
    shader->Set();

    BLEND->Set(true);
    RASTER->Set(D3D11_CULL_NONE);
    D3D->GetDC()->Draw(vertices.size(), 0);
    RASTER->Set(D3D11_CULL_BACK);
    BLEND->Set(false);
}

void SlashTrail::RenderDetail()
{
    if (ImGui::Button("PLAY"))
    {
        Play();
    }
    ImGui::SameLine();
    if (ImGui::Button("STOP"))
    {
        Stop();
    }

    ImGui::SliderFloat("interval", &interval, 0.0f, 0.1);

    ImGui::SliderInt("maxTrail", &maxTrail, 1, 100);
    material->RenderDetail();

}
