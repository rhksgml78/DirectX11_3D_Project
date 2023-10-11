#include "framework.h"

Billboard* Billboard::Create(string name)
{
    Billboard* temp = new Billboard();
    temp->name = name;
    temp->type = ObType::Billboard;
    temp->mesh = RESOURCE->meshes.Load("9.Billboard.mesh");
    temp->shader = RESOURCE->shaders.Load("9.Cube.hlsl");
    temp->shader->LoadGeometry();
    return temp;
}

void Billboard::Update()
{
    Actor::Update();
}

void Billboard::Render()
{
    VertexPSP* vertex = (VertexPSP*)mesh->vertices;
    vertex[0].size = Vector2(S._11, S._22);
    vertex[0].pivot = pivot;
    mesh->Update();
    Actor::Render();
}

void Billboard::RenderDetail()
{
    Actor::RenderDetail();
    if (ImGui::BeginTabBar("MyTabBar3"))
    {
        if (ImGui::BeginTabItem("Billboard"))
        {
            ImGui::SliderFloat2("pivot", (float*)&pivot, -1.0f, 1.0f);
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
}
