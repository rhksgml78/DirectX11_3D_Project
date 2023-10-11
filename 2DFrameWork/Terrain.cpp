#include "framework.h"
ID3D11ComputeShader* Terrain::computeShader = nullptr;
Actor* Terrain::Node = nullptr;
Actor* Terrain::Line = nullptr;
void Terrain::CreateStaticMember()
{
    Node = Actor::Create();
    Node->LoadFile("Sphere.xml");
    Line = Actor::Create();
    Line->LoadFile("Line.xml");



    ID3D10Blob* CsBlob;

    wstring path = L"../Shaders/ComputeShader.hlsl";

    DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;


    D3DCompileFromFile(path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "CS", "cs_5_0", flags, 0, &CsBlob, nullptr);

    D3D->GetDevice()->CreateComputeShader(CsBlob->GetBufferPointer(), CsBlob->GetBufferSize(),
        nullptr, &computeShader);
}

void Terrain::DeleteStaticMember()
{
    SafeRelease(computeShader);
}

Terrain* Terrain::Create(string Name, int terrainSize, float uvScale)
{
    Terrain* result = new Terrain(Name);
    result->uvScale = uvScale;
    result->triSize = (terrainSize - 1) * (terrainSize - 1) * 2;
    result->shader = RESOURCE->shaders.Load("5.Cube.hlsl");
    result->CreateMesh(terrainSize);
    result->material = new Material();
    result->material->normalMap = RESOURCE->textures.Load("20000001.png");
    result->material->ambient.w = 1.0f;

    return result;
}

Terrain::Terrain(string Name)
    : Actor()
{
    type = ObType::Terrain;
    name = Name;
    uvScale = 1.0f;
    showNode = false;
    depth = false;
}



void Terrain::CreateMesh(int Size)
{
    VertexTerrain* vertices;
    UINT* indices;
    UINT vertexCount;
    UINT indexCount;
    vector<VertexTerrain> VecVertex;
    vector<UINT> Vecindex;

    triSize = (Size - 1) * (Size - 1) * 2;
    float half = Size * 0.5f;

    for (int z = 0; z < Size; z++)
    {
        for (int x = 0; x < Size; x++)
        {
            //배열에 들어갈 인덱스
            int Index = (z * Size) + x;
            float _x = x - half;
            float _z = -z + half;
            float _y = 0.0f;

            float _u = float(x) * uvScale / float(Size - 1);
            float _v = float(z) * uvScale / float(Size - 1);
            VertexTerrain temp;
            temp.uv = Vector2(_u, _v);
            temp.position = Vector3(_x, _y, _z);
            temp.weights.x = 1.0f;
            temp.normal = Vector3(0, 1, 0);
            VecVertex.push_back(temp);
        }
    }

    vertexCount = (UINT)VecVertex.size();



    vertices = new VertexTerrain[vertexCount];


    //사각형 갯수만큼 반복문
    //사각형 갯수는  terrainSize - 1 *  terrainSize - 1
    //인덱스 갯수는  terrainSize - 1 *  terrainSize - 1 * 6
    for (int z = 0; z < Size - 1; z++)
    {
        for (int x = 0; x < Size - 1; x++)
        {
            //Vecindex.push_back(z* MaxZ + x);
            UINT Index = z * Size + x;
            //0
            Vecindex.push_back(Index);

            //1
            Vecindex.push_back(Index + 1);

            //2
            Vecindex.push_back(Index + Size + 1);


            //0
            Vecindex.push_back(Index);

            //1
            Vecindex.push_back(Index + Size + 1);

            //2
            Vecindex.push_back(Index + Size);
        }
    }
    indexCount = (UINT)Vecindex.size();
    indices = new UINT[indexCount];


    //벡터에 있던 내용 전체 복사

    copy(VecVertex.begin(), VecVertex.end(),
        stdext::checked_array_iterator<VertexTerrain*>(vertices, vertexCount));
    copy(Vecindex.begin(), Vecindex.end(),
        stdext::checked_array_iterator<UINT*>(indices, indexCount));


    SafeReset(mesh);
    mesh = make_shared<Mesh>(vertices, vertexCount, indices,
        indexCount, VertexType::TERRAIN);

    if (srv)
    {
        DeleteStructuredBuffer();
        CreateStructuredBuffer();
    }
}

void Terrain::UpdateMeshUv()
{
    VertexTerrain* vertices = (VertexTerrain*)mesh->vertices;
    int terrainSize = (int)sqrt(mesh->vertexCount);
    for (int z = 0; z < terrainSize; z++)
    {
        for (int x = 0; x < terrainSize; x++)
        {
            //배열에 들어갈 인덱스
            float _u = float(x) * uvScale / float(terrainSize - 1);
            float _v = float(z) * uvScale / float(terrainSize - 1);
            vertices[z * terrainSize + x].uv = Vector2(_u, _v);
        }
    }
    mesh->Update();
}
void Terrain::UpdateMeshNormal()
{
    VertexTerrain* vertices = (VertexTerrain*)mesh->vertices;

    for (UINT i = 0; i < mesh->vertexCount; i++)
    {
        vertices[i].normal = Vector3();
    }

    //삼각형 갯수만큼 반복
    for (UINT i = 0; i < mesh->indexCount / 3; i++)
    {
        //하나의 삼각형
        UINT index0 = mesh->indices[i * 3 + 0];
        UINT index1 = mesh->indices[i * 3 + 1];
        UINT index2 = mesh->indices[i * 3 + 2];

        Vector3 v0 = vertices[index0].position;
        Vector3 v1 = vertices[index1].position;
        Vector3 v2 = vertices[index2].position;

        Vector3 A = v1 - v0;
        Vector3 B = v2 - v0;

        Vector3 normal = A.Cross(B);
        //중첩시킬 벡터를 정규화해야 평균값
        normal.Normalize();

        vertices[index0].normal += normal;
        vertices[index1].normal += normal;
        vertices[index2].normal += normal;

    }
    mesh->Update();
}
void Terrain::CreateStructuredBuffer()
{
    //삼각형 단위
    inputArray = new InputDesc[triSize];
    for (UINT i = 0; i < triSize; i++)
    {
        UINT index0 = mesh->indices[i * 3 + 0];
        UINT index1 = mesh->indices[i * 3 + 1];
        UINT index2 = mesh->indices[i * 3 + 2];

        inputArray[i].v0 = ((VertexTerrain*)(mesh->vertices))[index0].position;
        inputArray[i].v1 = ((VertexTerrain*)(mesh->vertices))[index1].position;
        inputArray[i].v2 = ((VertexTerrain*)(mesh->vertices))[index2].position;

        inputArray[i].index = i;
    }
    //삼각형 단위
    outputArray = new OutputDesc[triSize];



    //input
    {
        ID3D11Buffer* buffer;
        D3D11_BUFFER_DESC desc = {};
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.ByteWidth = sizeof(InputDesc) * triSize;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
        desc.StructureByteStride = sizeof(InputDesc);

        D3D11_SUBRESOURCE_DATA initData = {};
        initData.pSysMem = inputArray;

        D3D->GetDevice()->CreateBuffer(&desc, &initData, &buffer);
        input = (ID3D11Resource*)buffer;


        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = DXGI_FORMAT_UNKNOWN;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
        srvDesc.BufferEx.FirstElement = 0;
        srvDesc.BufferEx.NumElements = triSize;
        //srv
        D3D->GetDevice()->CreateShaderResourceView(buffer, &srvDesc, &srv);

    }
    //output
    {
        ID3D11Buffer* buffer;
        D3D11_BUFFER_DESC desc = {};
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.ByteWidth = sizeof(OutputDesc) * triSize;
        desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
        desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
        desc.StructureByteStride = sizeof(OutputDesc);

        D3D->GetDevice()->CreateBuffer(&desc, nullptr, &buffer);
        //ID3D11Resource
        output = (ID3D11Resource*)buffer;


        D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
        uavDesc.Format = DXGI_FORMAT_UNKNOWN;
        uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
        uavDesc.Buffer.FirstElement = 0;
        uavDesc.Buffer.NumElements = triSize;
        //uav
        D3D->GetDevice()->CreateUnorderedAccessView(buffer, &uavDesc, &uav);
    }
    //result read
    {
        ID3D11Buffer* buffer;
        D3D11_BUFFER_DESC desc = {};
        ((ID3D11Buffer*)output)->GetDesc(&desc);
        desc.Usage = D3D11_USAGE_STAGING;
        desc.BindFlags = 0;
        desc.MiscFlags = 0;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

        D3D->GetDevice()->CreateBuffer(&desc, nullptr, &buffer);

        result = (ID3D11Resource*)buffer;

    }
    {
        D3D11_BUFFER_DESC desc = { 0 };
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.ByteWidth = sizeof(RayDesc);
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

        HRESULT hr = D3D->GetDevice()->CreateBuffer(&desc, NULL, &rayBuffer);
        assert(SUCCEEDED(hr));
    }
}

void Terrain::DeleteStructuredBuffer()
{
    input->Release();
    srv->Release();
    output->Release();
    uav->Release();
    result->Release();
    rayBuffer->Release();
    delete[] inputArray;
    delete[] outputArray;
}

bool Terrain::ComPutePicking(Ray WRay, OUT Vector3& HitPoint)
{
    //쉐이더부터 준비
    D3D->GetDC()->CSSetShader(computeShader, 0, 0);

    //raybuffer binding
    ray.position = WRay.position;
    ray.direction = WRay.direction;
    ray.size = (float)triSize;//삼각형갯수
    Matrix inverse = W.Invert();
    ray.direction = Vector3::TransformNormal(ray.direction, inverse);
    ray.direction.Normalize();
    ray.position = Vector3::Transform(ray.position, inverse);

    //트랜스폼한 Ray를 상수버퍼로 바인딩
    D3D->GetDC()->UpdateSubresource(rayBuffer, 0, NULL, &ray, 0, 0);

    D3D->GetDC()->CSSetConstantBuffers(0, 1, &rayBuffer);

    //input binding
    D3D->GetDC()->CSSetShaderResources(0, 1, &srv);
    //output binding
    D3D->GetDC()->CSSetUnorderedAccessViews(0, 1, &uav, nullptr);

    //멀티스레딩
    //shader 실행
    //올림
    UINT x = (UINT)ceil((float)triSize / 1024.0f);
    D3D->GetDC()->Dispatch(x, 1, 1);

    //동기화

    //gpu -> cpu 복사
    D3D->GetDC()->CopyResource(result, output);

    D3D11_MAPPED_SUBRESOURCE subResource;

    D3D->GetDC()->Map(result, 0, D3D11_MAP_READ, 0, &subResource);
    memcpy(outputArray, subResource.pData, sizeof(OutputDesc) * triSize);
    D3D->GetDC()->Unmap(result, 0);

    float minDistance = FLT_MAX;
    int minIndex = -1;
    for (int i = 0; i < triSize; i++)
    {
        OutputDesc temp = outputArray[i];
        if (temp.picked)
        {
            if (minDistance > temp.distance)
            {
                minDistance = temp.distance;
                minIndex = i;
            }
        }
    }
    if (minIndex >= 0)
    {
        HitPoint = ray.position + ray.direction
            * minDistance;
        HitPoint = Vector3::Transform(HitPoint, W);
        return true;
    }

    return false;
}

void Terrain::Render()
{
    Actor::Render();
    //다익스트라 노드 렌더링
    if (showNode)
    {
        if (depth)
            DEPTH->Set(false);
        for (auto it = di.list.begin(); it != di.list.end(); it++)
        {
            Node->SetLocalPos(it->second->pos);
            Node->Update();
            //local * srt;
            Node->W *= W;
            Node->W = S.Invert() * Node->W;

            /* for (auto it2 : Node->children) it2.second->Update();
             if (Node->collider) Node->collider->Update(Node);*/

            Node->Render();


            for (auto it2 = it->second->linkedList.begin();
                it2 != it->second->linkedList.end(); it2++)
            {
                Line->mesh->SetVertexPosition(0) = it->second->pos;
                Line->mesh->SetVertexPosition(1) = di.list[it2->first]->pos;
                Line->mesh->Update();
                Line->W = W;
                Line->Render();
            }
        }
        if (depth)
            DEPTH->Set(true);
    }
}

void Terrain::Render(shared_ptr<Shader> shader)
{
    Actor::Render(shader);

}

bool Terrain::PathFinding(int from, int to, std::list<Vector3 >& way)
{
   
    bool result = di.PathFinding(from, to, way);
    for (auto it = way.begin(); it != way.end(); it++)
    {
        *it = Vector3::Transform(*it, W);
    }
    return result;
}

void Terrain::AllPathActivate() const
{
    for (auto it = di.list.begin(); it != di.list.end(); it++)
    {
        for (auto it2 = it->second->linkedList.begin(); it2 != it->second->linkedList.end(); it2++)
        {
            it2->second.isActive = true;
        }
    }
}

bool Terrain::IntersectCol(Ray ray) const
{
    for (auto it = obList.begin(); it != obList.end(); it++)
    {
        Vector3 hit;
        if (it->second->Intersect(ray, hit))
        {
            float len = ray.direction.Length();
            float len2 = (hit - ray.position).Length();
            if (len > len2)
            {
                return true;
            }
        }
    }

    return false;
}

int Terrain::FindNodeIdx(Vector3 pos) const
{
    float dis = FLT_MAX;
    int idx = 0;
    for (auto it = di.list.begin(); it != di.list.end(); it++)
    {
        float dis2 = (it->second->pos - pos).Length();
        if (dis > dis2)
        {
            dis = dis2;
            idx = it->first;
        }
    }

    return idx;
}

void Terrain::AllLink(Vector3 pos)
{
    int num = FindNodeIdx(pos);

    for (auto it = di.list.begin(); it != di.list.end(); it++)
    {
        di.LinkNode(num, it->first);
    }
}

int  Terrain::GetDiNodeIdx(Vector3 pos)
{
    int idx = 0;
    float min = FLT_MAX;
    for (auto it = di.list.begin(); it != di.list.end(); it++)
    {
        Vector3 minus = it->second->pos - pos;

        if (min > minus.Length())
        {
            min = minus.Length();
            idx = it->first;
        }
    }
    return idx;
}

void Terrain::AutoNode(Vector3 localPos)
{
    //모든 노드로 부터 레이캐스팅( 길이제한)

    DiNode* temp = di.AddNode(localPos);

    for (auto it = di.list.begin(); it != di.list.end(); it++)
    {
        if (temp->GetId() == it->second->GetId())continue;

        Ray WRay;
        WRay.position = localPos;
        WRay.direction = it->second->pos - localPos;
        Vector3::Transform(WRay.position, W);
        Vector3::TransformNormal(WRay.direction, W);

        if (not IntersectCollider(WRay))
        {
            di.LinkNode(temp->GetId(), it->second->GetId());
        }
    }



}

bool Terrain::IntersectCollider(Ray WRay)
{
    bool result = false;
    float len = WRay.direction.Length();

    for (auto it = obList.begin(); it != obList.end(); it++)
    {
        Vector3 hit;

        if (it->second->Intersect(WRay, hit))
        {
            Vector3 dis = WRay.position - hit;
            float len2 = dis.Length();
            if (len > len2)
            {
                result = true;
            }
        }
    }
    return result;
}
