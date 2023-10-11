#include "framework.h"

Mesh::Mesh()
{
    //인스턴싱 사용을 위해 추가한 것들 초기화
    instance = nullptr;
    instanceBuffer = nullptr;
    instanceCount = 0;

    vertexType = VertexType::MODEL;
    primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
    byteWidth = sizeof(VertexModel);
    vertexCount = 4;
    indexCount = 4;

    VertexModel* Vertex = new VertexModel[vertexCount];
    indices = new UINT[indexCount];

    Vertex[0].position = Vector3(-1, 0, -1);
    Vertex[0].uv = Vector2(0, 20);
    Vertex[0].normal = Vector3(0, 1, 0);
    Vertex[0].tangent = Vector3(0, 0, 1);

    Vertex[1].position = Vector3(-1, 0, 1);
    Vertex[1].uv = Vector2(0, 0);
    Vertex[1].normal = Vector3(0, 1, 0);
    Vertex[1].tangent = Vector3(0, 0, 1);

    Vertex[2].position = Vector3(1, 0, -1);
    Vertex[2].uv = Vector2(20, 20);
    Vertex[2].normal = Vector3(0, 1, 0);
    Vertex[2].tangent = Vector3(0, 0, 1);

    Vertex[3].position = Vector3(1, 0, 1);
    Vertex[3].uv = Vector2(20, 0);
    Vertex[3].normal = Vector3(0, 1, 0);
    Vertex[3].tangent = Vector3(0, 0, 1);


    indices = new UINT[indexCount];

    //앞면
    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 3;









    vertices = Vertex;

    //CreateVertexBuffer
    {
        D3D11_BUFFER_DESC desc;
        desc = { 0 };
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.ByteWidth = byteWidth * vertexCount;
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        D3D11_SUBRESOURCE_DATA data = { 0 };
        data.pSysMem = vertices;

        HRESULT hr = D3D->GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
        assert(SUCCEEDED(hr));
    }

    //Create Index Buffer
    {
        D3D11_BUFFER_DESC desc;
        ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
        desc.ByteWidth = sizeof(UINT) * indexCount;
        desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

        D3D11_SUBRESOURCE_DATA data = { 0 };
        data.pSysMem = indices;

        HRESULT hr = D3D->GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
        assert(SUCCEEDED(hr));
    }
}

Mesh::Mesh(void* vertices, UINT vertexCount, UINT* indices, UINT indexCount, VertexType type)
{
    //인스턴싱 사용을 위해 추가한 것들 초기화
    instance = nullptr;
    instanceBuffer = nullptr;
    instanceCount = 0;

    vertexType = type;
    primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

    switch (type)
    {
    case VertexType::P:
        byteWidth = sizeof(VertexP);
        break;
    case VertexType::PC:
        byteWidth = sizeof(VertexPC);
        break;
    case VertexType::PCN:
        byteWidth = sizeof(VertexPCN);
        break;
    case VertexType::PTN:
        byteWidth = sizeof(VertexPTN);
        break;
    case VertexType::MODEL:
        byteWidth = sizeof(VertexModel);
        break;

    case VertexType::TERRAIN:
        byteWidth = sizeof(VertexTerrain);
        break;
    case VertexType::PT:
        byteWidth = sizeof(VertexPT);
        break;
    case VertexType::PS:
        byteWidth = sizeof(VertexPS);
        break;
    case VertexType::PSV:
        byteWidth = sizeof(VertexPSV);
        break;
    case VertexType::PSP:
        byteWidth = sizeof(VertexPSP);
        break;
    }
    ////////////////////////////////////////////////////

    ///////////////////////////////////////////////////
    this->vertices = vertices;
    this->vertexCount = vertexCount;
    this->indices = indices;
    this->indexCount = indexCount;

    //CreateVertexBuffer
    {
        D3D11_BUFFER_DESC desc;
        desc = { 0 };
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.ByteWidth = byteWidth * vertexCount;
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        D3D11_SUBRESOURCE_DATA data = { 0 };
        data.pSysMem = vertices;

        HRESULT hr = D3D->GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
        assert(SUCCEEDED(hr));
    }

    //Create Index Buffer
    {
        D3D11_BUFFER_DESC desc;
        ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
        desc.ByteWidth = sizeof(UINT) * indexCount;
        desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

        D3D11_SUBRESOURCE_DATA data = { 0 };
        data.pSysMem = indices;

        HRESULT hr = D3D->GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
        assert(SUCCEEDED(hr));
    }
}

Mesh::~Mesh()
{
    /*switch (vertexType)
    {
    case VertexType::P:
        if (vertices)delete[](VertexP*)vertices;
        break;
    case VertexType::PC:
        if (vertices)delete[](VertexPC*)vertices;
        break;
    case VertexType::PCN:
        if (vertices)delete[](VertexPCN*)vertices;
        break;
    case VertexType::PTN:
        if (vertices)delete[](VertexPTN*)vertices;
        break;
    case VertexType::MODEL:
        if (vertices)delete[](VertexModel*)vertices;
        break;
    case VertexType::TERRAIN:
        if (vertices)delete[](VertexTerrain*)vertices;
        break;
    case VertexType::PT:
        if (vertices)delete[](VertexPT*)vertices;
        break;
    case VertexType::PS:
        if (vertices)delete[](VertexPS*)vertices;
        break;
    case VertexType::PSV:
        if (vertices)delete[](VertexPSV*)vertices;
        break;
    }
    delete[] indices;*/
}

void Mesh::Set()
{
    UINT offset = 0;
    D3D->GetDC()->IASetVertexBuffers(0,
        1,
        &vertexBuffer,
        &byteWidth,
        &offset);
    D3D->GetDC()->IASetPrimitiveTopology
    (primitiveTopology);
    D3D->GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
}

void Mesh::LoadFile(string file)
{
    this->file = file;
    BinaryReader in;
    wstring path = L"../Contents/Mesh/" + Util::ToWString(file);
    in.Open(path);
    //읽기전
    switch (vertexType)
    {
    case VertexType::P:
        if (vertices)delete[](VertexP*)vertices;
        break;
    case VertexType::PC:
        if (vertices)delete[](VertexPC*)vertices;
        break;
    case VertexType::PCN:
        if (vertices)delete[](VertexPCN*)vertices;
        break;
    case VertexType::PTN:
        if (vertices)delete[](VertexPTN*)vertices;
        break;
    case VertexType::MODEL:
        if (vertices)delete[](VertexModel*)vertices;
        break;
    case VertexType::TERRAIN:
        if (vertices)delete[](VertexTerrain*)vertices;
        break;
    case VertexType::PT:
        if (vertices)delete[](VertexPT*)vertices;
        break;
    case VertexType::PS:
        if (vertices)delete[](VertexPS*)vertices;
        break;
    case VertexType::PSV:
        if (vertices)delete[](VertexPSV*)vertices;
        break;
    }
    vertexType = (VertexType)in.UInt();
    primitiveTopology = (D3D_PRIMITIVE_TOPOLOGY)in.UInt();
    byteWidth = in.UInt();
    vertexCount = in.UInt();
    indexCount = in.UInt();

    SafeDeleteArray(indices);
    indices = new UINT[indexCount];


    //읽고난후
    switch (vertexType)
    {
    case VertexType::P:
    {
        vertices = new VertexP[vertexCount];
        VertexP* vertex = (VertexP*)vertices;
        for (UINT i = 0; i < vertexCount; i++)
        {
            vertex[i].position = in.vector3();
        }
        break;
    }

    case VertexType::PC:
    {
        vertices = new VertexPC[vertexCount];
        VertexPC* vertex = (VertexPC*)vertices;
        for (UINT i = 0; i < vertexCount; i++)
        {
            vertex[i].position = in.vector3();
            vertex[i].color = in.color3f();
        }
        break;
    }
    case VertexType::PCN:
    {
        vertices = new VertexPCN[vertexCount];
        VertexPCN* vertex = (VertexPCN*)vertices;
        for (UINT i = 0; i < vertexCount; i++)
        {
            vertex[i].position = in.vector3();
            vertex[i].color = in.color3f();
            vertex[i].normal = in.vector3();
        }
        break;
    }
    case VertexType::PTN:
    {
        vertices = new VertexPTN[vertexCount];
        VertexPTN* vertex = (VertexPTN*)vertices;
        for (UINT i = 0; i < vertexCount; i++)
        {
            vertex[i].position = in.vector3();
            vertex[i].uv = in.vector2();
            vertex[i].normal = in.vector3();
        }
        break;
    }
    case VertexType::MODEL:
    {
        vertices = new VertexModel[vertexCount];
        VertexModel* vertex = (VertexModel*)vertices;
        for (UINT i = 0; i < vertexCount; i++)
        {
            vertex[i].position = in.vector3();
            vertex[i].uv = in.vector2();
            vertex[i].normal = in.vector3();
            vertex[i].tangent = in.vector3();
            vertex[i].indices = in.vector4();
            vertex[i].weights = in.vector4();
        }
        break;
    }
    case VertexType::TERRAIN:
    {
        vertices = new VertexTerrain[vertexCount];
        VertexTerrain* vertex = (VertexTerrain*)vertices;
        for (UINT i = 0; i < vertexCount; i++)
        {
            vertex[i].position = in.vector3();
            vertex[i].uv = in.vector2();
            vertex[i].normal = in.vector3();
            vertex[i].weights = in.vector4();
        }
        break;
    }
    case VertexType::PT:
    {
        vertices = new VertexPT[vertexCount];
        VertexPT* vertex = (VertexPT*)vertices;
        for (UINT i = 0; i < vertexCount; i++)
        {
            vertex[i].position = in.vector3();
            vertex[i].uv = in.vector2();
        }
        break;
    }
    case VertexType::PS:
    {
        vertices = new VertexPS[vertexCount];
        VertexPS* vertex = (VertexPS*)vertices;
        for (UINT i = 0; i < vertexCount; i++)
        {
            vertex[i].position = in.vector3();
            vertex[i].size = in.vector2();
        }
        break;
    }
    case VertexType::PSV:
    {
        vertices = new VertexPSV[vertexCount];
        VertexPSV* vertex = (VertexPSV*)vertices;
        for (UINT i = 0; i < vertexCount; i++)
        {
            vertex[i].position = in.vector3();
            vertex[i].size = in.vector2();
            vertex[i].velocity = in.vector3();
        }
        break;
    }
    }
    for (UINT i = 0; i < indexCount; i++)
    {
        indices[i] = in.UInt();
    }
    in.Close();

    SafeRelease(vertexBuffer);
    SafeRelease(indexBuffer);
    //CreateVertexBuffer
    {
        D3D11_BUFFER_DESC desc;
        desc = { 0 };
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.ByteWidth = byteWidth * vertexCount;
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        D3D11_SUBRESOURCE_DATA data = { 0 };
        data.pSysMem = vertices;

        HRESULT hr = D3D->GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
        assert(SUCCEEDED(hr));
    }

    //Create Index Buffer
    {
        D3D11_BUFFER_DESC desc;
        ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
        desc.ByteWidth = sizeof(UINT) * indexCount;
        desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

        D3D11_SUBRESOURCE_DATA data = { 0 };
        data.pSysMem = indices;

        HRESULT hr = D3D->GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
        assert(SUCCEEDED(hr));
    }

}

void Mesh::SaveFile(string file)
{
    this->file = file;

    BinaryWriter out;
    wstring path = L"../Contents/Mesh/" + Util::ToWString(file);
    out.Open(path);

    out.UInt((UINT)vertexType);
    out.UInt((UINT)primitiveTopology);
    out.UInt(byteWidth);
    out.UInt(vertexCount);
    out.UInt(indexCount);

    switch (vertexType)
    {
    case VertexType::P:
    {
        VertexP* vertex = (VertexP*)vertices;
        for (UINT i = 0; i < vertexCount; i++)
        {
            out.vector3(vertex[i].position);
        }
        break;
    }

    case VertexType::PC:
    {
        VertexPC* vertex = (VertexPC*)vertices;
        for (UINT i = 0; i < vertexCount; i++)
        {
            out.vector3(vertex[i].position);
            out.color3f(vertex[i].color);
        }
        break;
    }
    case VertexType::PCN:
    {
        VertexPCN* vertex = (VertexPCN*)vertices;
        for (UINT i = 0; i < vertexCount; i++)
        {
            out.vector3(vertex[i].position);
            out.color3f(vertex[i].color);
            out.vector3(vertex[i].normal);
        }
        break;
    }
    case VertexType::PTN:
    {
        VertexPTN* vertex = (VertexPTN*)vertices;
        for (UINT i = 0; i < vertexCount; i++)
        {
            out.vector3(vertex[i].position);
            out.vector2(vertex[i].uv);
            out.vector3(vertex[i].normal);
        }
        break;
    }
    case VertexType::MODEL:
    {
        VertexModel* vertex = (VertexModel*)vertices;
        for (UINT i = 0; i < vertexCount; i++)
        {
            out.vector3(vertex[i].position);
            out.vector2(vertex[i].uv);
            out.vector3(vertex[i].normal);
            out.vector3(vertex[i].tangent);
            out.vector4(vertex[i].indices);
            out.vector4(vertex[i].weights);
        }
        break;
    }
    case VertexType::TERRAIN:
    {
        VertexTerrain* vertex = (VertexTerrain*)vertices;
        for (UINT i = 0; i < vertexCount; i++)
        {
            out.vector3(vertex[i].position);
            out.vector2(vertex[i].uv);
            out.vector3(vertex[i].normal);
            out.vector4(vertex[i].weights);
        }
        break;
    }
    case VertexType::PT:
    {
        VertexPT* vertex = (VertexPT*)vertices;
        for (UINT i = 0; i < vertexCount; i++)
        {
            out.vector3(vertex[i].position);
            out.vector2(vertex[i].uv);
        }
        break;
    }
    case VertexType::PS:
    {
        VertexPS* vertex = (VertexPS*)vertices;
        for (UINT i = 0; i < vertexCount; i++)
        {
            out.vector3(vertex[i].position);
            out.vector2(vertex[i].size);
        }
        break;
    }
    case VertexType::PSV:
    {
        VertexPSV* vertex = (VertexPSV*)vertices;
        for (UINT i = 0; i < vertexCount; i++)
        {
            out.vector3(vertex[i].position);
            out.vector2(vertex[i].size);
            out.vector3(vertex[i].velocity);
        }
        break;
    }
    }
    for (UINT i = 0; i < indexCount; i++)
    {
        out.UInt(indices[i]);
    }
    out.Close();
}

void Mesh::LoadInstanceFile(string file)
{
    instanceFile = file;
    BinaryReader in;
    wstring path = L"../Contents/Instance/" + Util::ToWString(file);
    in.Open(path);
    instanceCount = in.Int();
    SafeDeleteArray(instance);
    instance = new Matrix[instanceCount];

    for (int i = 0; i < instanceCount; i++)
    {
        instance[i] = in.matrix();
    }
    in.Close();

    CreateInstanceBuffer(instance, instanceCount);
}

void Mesh::SaveInstanceFile(string file)
{
    instanceFile = file;
    BinaryWriter out;
    wstring path = L"../Contents/Instance/" + Util::ToWString(file);
    out.Open(path);
    out.Int(instanceCount);
    for (int i = 0; i < instanceCount; i++)
    {
        out.matrix(instance[i]);
    }
    out.Close();
}

void Mesh::Reset()
{
    SafeRelease(indexBuffer);
    SafeRelease(vertexBuffer);
    //CreateVertexBuffer
    {
        D3D11_BUFFER_DESC desc;
        desc = { 0 };
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.ByteWidth = byteWidth * vertexCount;
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        D3D11_SUBRESOURCE_DATA data = { 0 };
        data.pSysMem = vertices;

        HRESULT hr = D3D->GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
        assert(SUCCEEDED(hr));
    }

    //Create Index Buffer
    {
        D3D11_BUFFER_DESC desc;
        ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
        desc.ByteWidth = sizeof(UINT) * indexCount;
        desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

        D3D11_SUBRESOURCE_DATA data = { 0 };
        data.pSysMem = indices;

        HRESULT hr = D3D->GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
        assert(SUCCEEDED(hr));
    }
}

const Vector3& Mesh::GetVertexPosition(UINT idx)
{
    if (vertexType == VertexType::PC)
    {
        VertexPC* Vertices = (VertexPC*)vertices;
        return Vertices[indices[idx]].position;
    }
    else if (vertexType == VertexType::PCN)
    {
        VertexPCN* Vertices = (VertexPCN*)vertices;
        return Vertices[indices[idx]].position;
    }
    else if (vertexType == VertexType::PTN)
    {
        VertexPTN* Vertices = (VertexPTN*)vertices;
        return Vertices[indices[idx]].position;
    }
    else if (vertexType == VertexType::MODEL)
    {
        VertexModel* Vertices = (VertexModel*)vertices;
        return Vertices[indices[idx]].position;
    }
    else if (vertexType == VertexType::TERRAIN)
    {
        VertexTerrain* Vertices = (VertexTerrain*)vertices;
        return Vertices[indices[idx]].position;
    }
    else if (vertexType == VertexType::PT)
    {
        VertexPT* Vertices = (VertexPT*)vertices;
        return Vertices[indices[idx]].position;
    }
    else if (vertexType == VertexType::PS)
    {
        VertexPS* Vertices = (VertexPS*)vertices;
        return Vertices[indices[idx]].position;
    }
    else if (vertexType == VertexType::PSV)
    {
        VertexPSV* Vertices = (VertexPSV*)vertices;
        return Vertices[indices[idx]].position;
    }
    else if (vertexType == VertexType::PSP)
    {
        VertexPSP* Vertices = (VertexPSP*)vertices;
        return Vertices[indices[idx]].position;
    }

    VertexP* Vertices = (VertexP*)vertices;
    return Vertices[indices[idx]].position;
}

Vector3& Mesh::SetVertexPosition(UINT idx)
{
    if (vertexType == VertexType::PC)
    {
        VertexPC* Vertices = (VertexPC*)vertices;
        return Vertices[indices[idx]].position;
    }
    else if (vertexType == VertexType::PCN)
    {
        VertexPCN* Vertices = (VertexPCN*)vertices;
        return Vertices[indices[idx]].position;
    }
    else if (vertexType == VertexType::PTN)
    {
        VertexPTN* Vertices = (VertexPTN*)vertices;
        return Vertices[indices[idx]].position;
    }
    else if (vertexType == VertexType::MODEL)
    {
        VertexModel* Vertices = (VertexModel*)vertices;
        return Vertices[indices[idx]].position;
    }
    else if (vertexType == VertexType::TERRAIN)
    {
        VertexTerrain* Vertices = (VertexTerrain*)vertices;
        return Vertices[indices[idx]].position;
    }
    else if (vertexType == VertexType::PT)
    {
        VertexPT* Vertices = (VertexPT*)vertices;
        return Vertices[indices[idx]].position;
    }
    else if (vertexType == VertexType::PS)
    {
        VertexPS* Vertices = (VertexPS*)vertices;
        return Vertices[indices[idx]].position;
    }
    else if (vertexType == VertexType::PSV)
    {
        VertexPSV* Vertices = (VertexPSV*)vertices;
        return Vertices[indices[idx]].position;
    }
    else if (vertexType == VertexType::PSP)
    {
        VertexPSP* Vertices = (VertexPSP*)vertices;
        return Vertices[indices[idx]].position;
    }

    VertexP* Vertices = (VertexP*)vertices;
    return Vertices[indices[idx]].position;
}

void Mesh::Update()
{
    D3D->GetDC()->UpdateSubresource
    (vertexBuffer, 0, NULL, vertices, 0, 0);
}

// 인스턴스 버퍼 생성하기
void Mesh::CreateInstanceBuffer(Matrix* instance, UINT instanceCount)
{
    this->instance = instance;
    this->instanceCount = instanceCount;
    // 이전의 버퍼가 있다면 지워주고 만들수있도록
    SafeRelease(instanceBuffer);

    //CreateInstanceBuffer
    {
        D3D11_BUFFER_DESC desc;
        desc = { 0 };
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.ByteWidth = sizeof(Matrix) * instanceCount;
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        D3D11_SUBRESOURCE_DATA data = { 0 };
        data.pSysMem = instance;
        HRESULT hr = D3D->GetDevice()->CreateBuffer(&desc, &data, &instanceBuffer);
        assert(SUCCEEDED(hr));
    }

}

void Mesh::DrawCall() // 드로우콜의 방식을 변경
{
    if (not instanceBuffer) // 인스턴싱 버퍼가 없다면
    {
        // 기존에 게임오브젝트렌더나 콜라이더 렌더에있던 것처럼 그대로 렌더
        D3D->GetDC()->DrawIndexed(indexCount, 0, 0);
    }
    else // 인스턴싱 버퍼가 있다면
    {
        UINT temp = sizeof(Matrix);
        UINT offset = 0;
        // IASetVertexBuffers 왜 인스턴스버퍼가아닌가? 따로 인스턴스버퍼가없어서 사용하는것이다.
        D3D->GetDC()->IASetVertexBuffers(1, 1, &instanceBuffer, &temp, &offset);
        // 인덱스의 수만필요한것이아닌 몇개의 인스턴스를 그릴것인지도 포함된다.
        D3D->GetDC()->DrawIndexedInstanced(indexCount, instanceCount, 0, 0, 0);
    }
}

void Mesh::RenderDetail()
{
    if (ImGui::Button("InstanceEdit"))
    {
        GUI->mesh = this;
    }
}

void Mesh::InstanceEdit()
{
    string instanceCount = to_string(this->instanceCount);
    ImGui::Text(instanceCount.c_str());

    for (int i = 0; i < this->instanceCount; i++)
    {
        ImGui::Text("X: %f Y: %f Z: %f", instance[i]._14, instance[i]._24, instance[i]._34);
        string instanceCount = "Instance" + to_string(i) + "paste";
        if (ImGui::Button(instanceCount.c_str()))
        {
            instance[i] = GUI->World;
            D3D->GetDC()->UpdateSubresource
            (instanceBuffer, 0, NULL, instance, 0, 0);
        }
    }
    if (ImGui::Button("+"))
    {
        Matrix* Instance = new Matrix[this->instanceCount + 1];
        //복사
        memcpy(Instance, instance, sizeof(Matrix) * this->instanceCount);

        CreateInstanceBuffer(Instance, this->instanceCount + 1);

        //this->instanceCount += 1;
    }
    if (ImGui::Button("-"))
    {
        if (this->instanceCount > 1)
        {
            Matrix* Instance = new Matrix[this->instanceCount - 1];
            //복사
            memcpy(Instance, instance, sizeof(Matrix) * (this->instanceCount - 1));

            CreateInstanceBuffer(Instance, this->instanceCount - 1);

            //this->instanceCount--;
        }
        else if (this->instanceCount == 1)
        {
            this->instanceCount = 0;
            SafeDeleteArray(instance);
            SafeRelease(instanceBuffer);
        }
    }

    if (GUI->FileImGui("Save", "Save Instance",
        ".ins", "../Contents/Instance"))
    {
        string path = ImGuiFileDialog::Instance()->GetCurrentPath();
        Util::Replace(&path, "\\", "/");
        if (path.find("/Instance/") != -1)
        {
            size_t tok = path.find("/Instance/") + 10;
            path = path.substr(tok, path.length())
                + "/" + ImGuiFileDialog::Instance()->GetCurrentFileName();
        }
        else
        {
            path = ImGuiFileDialog::Instance()->GetCurrentFileName();
        }
        SaveInstanceFile(path);
    }
    ImGui::SameLine();
    if (GUI->FileImGui("Load", "Load Instance",
        ".ins", "../Contents/Instance"))
    {
        string path = ImGuiFileDialog::Instance()->GetCurrentPath();
        Util::Replace(&path, "\\", "/");
        if (path.find("/Instance/") != -1)
        {
            size_t tok = path.find("/Instance/") + 10;
            path = path.substr(tok, path.length())
                + "/" + ImGuiFileDialog::Instance()->GetCurrentFileName();
        }
        else
        {
            path = ImGuiFileDialog::Instance()->GetCurrentFileName();
        }
        LoadInstanceFile(path);
    }


}
