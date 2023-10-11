#include "stdafx.h"
#include "Main.h"

Main::Main()
{
	D3D11_BUFFER_DESC desc = { 0 };
	desc.ByteWidth = sizeof(Brush);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//상수버퍼
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;
	HRESULT hr = D3D->GetDevice()->CreateBuffer(&desc, NULL, &brushBuffer);
	assert(SUCCEEDED(hr));
	D3D->GetDC()->PSSetConstantBuffers(10, 1, &brushBuffer);
}

Main::~Main(){}

void Main::Init()
{
	//sky = Sky::Create();

    Cam = Camera::Create();
    Cam->LoadFile("Cam.xml");
    Camera::main = Cam;

	Cam->width = App.GetWidth();
	Cam->height = App.GetHeight();
	Cam->viewport.width = App.GetWidth();
	Cam->viewport.height = App.GetHeight();

	terrain = Terrain::Create();
	terrain->LoadFile("DJ_Terrain.xml");
	terrain->CreateStructuredBuffer();

	mousePoint = Actor::Create();
	//mousePoint->LoadFile("Deadpool.xml");
	terrain->showNode = true;

	/*dungeon = Actor::Create();
	dungeon->LoadFile("DungeonStart.xml");

	dungeonend = Actor::Create();
	dungeonend->LoadFile("DungeonEnd.xml");

	wall = Actor::Create();
	wall->LoadFile("Wall00.xml");

	fieldwall = Actor::Create();
	fieldwall->LoadFile("FieldWall.xml");

	floor = Actor::Create();
	floor->LoadFile("Floor01.xml");

	LockBlock = Actor::Create();
	LockBlock->LoadFile("LockBlock.xml");

	saw = Actor::Create();
	saw->LoadFile("SawTooth.xml");

	shr = Actor::Create();
	shr->LoadFile("Shuriken.xml");

	droprock = Actor::Create();
	droprock->LoadFile("DropRock.xml");

	bosswall = Actor::Create();
	bosswall->LoadFile("BossWall.xml");

	bossgate = Actor::Create();
	bossgate->LoadFile("BossGate.xml");

	trap = Actor::Create();
	trap->LoadFile("Gear.xml");*/

	Camera::main->mainCamSpeed = 50.0f;
}

void Main::Release()
{
	Cam->Release();
	terrain->Release();
	/*dungeon->Release();
	dungeonend->Release();
	wall->Release();
	fieldwall->Release();
	floor->Release();
	LockBlock->Release();
	saw->Release();
	shr->Release();
	droprock->Release();
	bosswall->Release();
	bossgate->Release();
	trap->Release();
	sky->Release();*/
}

void Main::Update()
{
	LIGHT->RenderDetail();
	ImGui::SliderFloat("MainCamSpeed",&Camera::main->mainCamSpeed, 0.0f, 100.0f);

	static char temp[128] = "Terrain";
	ImGui::InputText("file", temp, 128);

	if (ImGui::Button("AllSave"))
	{
		string temp2 = temp;
		terrain->mesh->SaveFile(temp2 + ".mesh");
		terrain->material->SaveFile(temp2 + ".mtl");
		terrain->di.file = temp2 + ".di";
		terrain->SaveFile(temp2 + ".xml");
	}

	if (ImGui::Button("nodeEdit"))
	{
		nodeEdit = 1;
		heightBrush = 0;
		textureBrush =0;
		objEdit =0;
	}
	ImGui::SameLine();
	if (ImGui::Button("textureBrush"))
	{
		nodeEdit = 0;
		heightBrush = 0;
		textureBrush = 1;
		objEdit = 0;
	}
	ImGui::SameLine();
	if (ImGui::Button("heightBrush"))
	{
		nodeEdit = 0;
		heightBrush = 1;
		textureBrush = 0;
		objEdit = 0;
	}
	ImGui::SameLine();
	if (ImGui::Button("objEdit"))
	{
		nodeEdit = 0;
		heightBrush = 0;
		textureBrush = 0;
		objEdit = 1;
	}

	if (nodeEdit)
	{
		ImGui::Text("firstLink: %d  secondLink: %d", firstLink, secondLink);
		if (ImGui::Button("AddNode"))
		{
			nodeState = 0;
		}
		ImGui::SameLine();
		if (ImGui::Button("DeleteNode"))
		{
			nodeState = 1;
		}
		ImGui::SameLine();
		if (ImGui::Button("LinkNode"))
		{
			nodeState = 2;
		}
	}
	else
	{
		if (GUI->FileImGui("Load", "LoadRawFile",
			".raw,.RAW", "../Contents/Raw"))
		{
			string path = ImGuiFileDialog::Instance()->GetCurrentPath();
			Util::Replace(&path, "\\", "/");
			if (path.find("/Raw/") != -1)
			{
				size_t tok = path.find("/Raw/") + 5;
				path = path.substr(tok, path.length())
					+ "/" + ImGuiFileDialog::Instance()->GetCurrentFileName();
			}
			else
			{
				path = ImGuiFileDialog::Instance()->GetCurrentFileName();
			}
			string file = "../Contents/Raw/" + path;
			FILE* fp;
			//파일 열기
			fopen_s(&fp, file.c_str(), "rb");
			fseek(fp, 0, SEEK_END);
			int Size = ftell(fp);
			fseek(fp, 0, SEEK_SET);

			BYTE* Height = new BYTE[Size];

			for (int i = 0; i < Size; i++)
			{
				char c = fgetc(fp);
				Height[i] = c;
			}

			int terrainSize = (int)sqrt(Size);
			int triSize = (terrainSize - 1) * (terrainSize - 1) * 2;
			float half = terrainSize * 0.5f;

			VertexTerrain* vertices = new VertexTerrain[Size];

			for (int z = 0; z < terrainSize; z++)
			{
				for (int x = 0; x < terrainSize; x++)
				{
					int Index = (z * terrainSize) + x;
					float _x = x - half;
					float _z = -z + half;
					float _y = Height[Index] / 255.0f * (float)terrainSize;

					float _u = float(x) / float(terrainSize - 1);
					float _v = float(z) / float(terrainSize - 1);

					vertices[Index].uv = Vector2(_u, _v);
					vertices[Index].position = Vector3(_x, _y, _z);
					vertices[Index].normal = Vector3(0, 1, 0);
				}
			}

			UINT* indices = new UINT[triSize * 3];
			int Idx = 0;
			for (int z = 0; z < terrainSize - 1; z++)
			{
				for (int x = 0; x < terrainSize - 1; x++)
				{
					UINT Index = z * terrainSize + x;
					indices[Idx] = Index;
					Idx++;
					indices[Idx] = Index + 1;
					Idx++;
					indices[Idx] = Index + terrainSize + 1;
					Idx++;
					indices[Idx] = Index;
					Idx++;
					indices[Idx] = Index + terrainSize + 1;
					Idx++;
					indices[Idx] = Index + terrainSize;
					Idx++;
				}
			}
			terrain->mesh = make_shared<Mesh>(vertices, Size, indices, Idx,
				VertexType::TERRAIN);
			terrain->UpdateMeshNormal();

			//파일 닫기
			fclose(fp);
		}

		ImGui::Text("Select Brush");
		
		for (int i = 0; i < 3; i++)
		{
			string temp = "Brush" + to_string(i);
			if (ImGui::Button(temp.c_str()))
			{
				brushIdx = i;
			}
			if (i < 2)
				ImGui::SameLine();
		}

		ImGui::SliderFloat("brushRange", &brush[brushIdx].range, 1.0f, 100.0f);
		ImGui::SliderFloat("brushYScale", &brush[brushIdx].YScale, -100.0f, 100.0f);
		ImGui::SliderInt("brushType", &brush[brushIdx].type, 0, 3);
		if (ImGui::Button("[]"))
		{
			brush[brushIdx].shape = 1;
		}
		ImGui::SameLine();
		if (ImGui::Button("()"))
		{
			brush[brushIdx].shape = 0;
		}

		if (terrain->material->ambient.w)
		{
			ImVec2 size;
			size.x = 20; size.y = 20;
			if (ImGui::ImageButton((void*)terrain->material->normalMap->srv, size))
			{
				brush[brushIdx].texture = 0;
			}
			ImGui::SameLine();
		}
		if (terrain->material->diffuse.w)
		{
			ImVec2 size;
			size.x = 20; size.y = 20;
			if (ImGui::ImageButton((void*)terrain->material->diffuseMap->srv, size))
			{
				brush[brushIdx].texture = 1;
			}
			ImGui::SameLine();
		}
		if (terrain->material->specular.w)
		{
			ImVec2 size;
			size.x = 20; size.y = 20;
			if (ImGui::ImageButton((void*)terrain->material->specularMap->srv, size))
			{
				brush[brushIdx].texture = 2;
			}
			ImGui::SameLine();
		}
		if (terrain->material->emissive.w)
		{
			ImVec2 size;
			size.x = 20; size.y = 20;
			if (ImGui::ImageButton((void*)terrain->material->emissiveMap->srv, size))
			{
				brush[brushIdx].texture = 3;
			}
		}
	}
	
	Camera::ControlMainCam();

	ImGui::Text("FPS: %d", TIMER->GetFramePerSecond());
	ImGui::Begin("Hierarchy");
	Cam->RenderHierarchy();
	terrain->RenderHierarchy();
	mousePoint->RenderHierarchy();
	/*dungeon->RenderHierarchy();
	dungeonend->RenderHierarchy();
	floor->RenderHierarchy();
	wall->RenderHierarchy();
	fieldwall->RenderHierarchy();
	LockBlock->RenderHierarchy();
	saw->RenderHierarchy();
	shr->RenderHierarchy();
	droprock->RenderHierarchy();
	bosswall->RenderHierarchy();
	bossgate->RenderHierarchy();
	trap->RenderHierarchy();
	sky->RenderHierarchy();*/
	ImGui::End();

	Cam->Update();
	terrain->Update();
	mousePoint->Update();

	/*dungeon->Update();
	dungeonend->Update();
	floor->Update();
	wall->Update();
	fieldwall->Update();
	LockBlock->Update();
	saw->Update();
	shr->Update();
	droprock->Update();
	bosswall->Update();
	bossgate->Update();
	trap->Update();
	sky->Update();*/
}

void Main::LateUpdate()
{
	Vector3 Hit;
	if (terrain->ComPutePicking(Util::MouseToRay(), Hit))
	{
		_brush.point = Hit;
		mousePoint->SetWorldPos(Hit);




		if (INPUT->KeyDown(VK_MBUTTON))
		{
			Matrix inverse = terrain->W.Invert();
			Vector3 localPos = Vector3::Transform(Hit, inverse);
			if (nodeEdit)
			{
				if (nodeState == 0)
				{
					//terrain->AutoNode(localPos);
					terrain->di.AddNode(localPos);
				}
				if (nodeState == 1)
				{
					int temp = terrain->di.GetNode(localPos);
					if (temp)
					{
						terrain->di.PopNode(temp);
					}
				}
				if (nodeState == 2)
				{
					if (not firstLink)
					{
						firstLink = terrain->di.GetNode(localPos);
					}
					else
					{
						secondLink = terrain->di.GetNode(localPos);

						if (secondLink and firstLink)
						{
							terrain->di.LinkNode(firstLink, secondLink);
							secondLink = 0;
							firstLink = 0;
						}
					}

				}
			}
			if (objEdit)
			{
				int number = RANDOM->Int(INT_MIN, INT_MAX);

				GameObject* temp = GameObject::Create(to_string(number));
				if (temp)
				{
					terrain->AddChild(temp);
					temp->SetWorldPos(Hit);
					temp->collider = new Collider(ColliderType::BOX);
					GUI->target = temp;
				}

			}
		}


		if (INPUT->KeyPress(VK_MBUTTON) and (heightBrush or textureBrush))
		{
			VertexTerrain* vertices = (VertexTerrain*)terrain->mesh->vertices;


			Matrix Inverse = terrain->W.Invert();
			Hit = Vector3::Transform(Hit, Inverse);
			float YScale = brush[brushIdx].YScale / terrain->S._22;
			float Range = brush[brushIdx].range / terrain->S._11;




			for (UINT i = 0; i < terrain->mesh->vertexCount; i++)
			{
				Vector3 v1 = Vector3(Hit.x, 0.0f, Hit.z);
				Vector3 v2 = Vector3(vertices[i].position.x,
					0.0f, vertices[i].position.z);
				float w;
				Vector3 temp = v2 - v1;
				float Dis = temp.Length();
				if (brush[brushIdx].shape == 1)
				{
					if (fabs(v1.x - v2.x) < brush[brushIdx].range and
						fabs(v1.z - v2.z) < brush[brushIdx].range)
					{
						//nomalize
						w = Dis / (Range * 1.414f);
						// 0 ~ 1
						w = Util::Saturate(w);

						w = (1.0f - w);
					}
					else
					{
						w = 0.0f;
					}
				}
				if (brush[brushIdx].shape == 0)
				{
					//nomalize
					w = Dis / Range;
					// 0 ~ 1
					w = Util::Saturate(w);

					w = (1.0f - w);
				}


				if (brush[brushIdx].type == 1)
					w = sin(w * PI * 0.5f);

				if (brush[brushIdx].type == 2)
					w = w ? 1 : 0;
				if (brush[brushIdx].type == 3)
				{
					if (w)
					{
						w = Hit.y - vertices[i].position.y;
						w = Util::Saturate(w, -1.0f, 1.0f);
					}
				}

				if (textureBrush)
				{
					if (brush[brushIdx].texture == 0)
					{
						vertices[i].weights.x += w * YScale * DELTA;
						vertices[i].weights.y -= w * YScale * DELTA;
						vertices[i].weights.z -= w * YScale * DELTA;
						vertices[i].weights.w -= w * YScale * DELTA;
					}
					if (brush[brushIdx].texture == 1)
					{
						vertices[i].weights.x -= w * YScale * DELTA;
						vertices[i].weights.y += w * YScale * DELTA;
						vertices[i].weights.z -= w * YScale * DELTA;
						vertices[i].weights.w -= w * YScale * DELTA;
					}
					if (brush[brushIdx].texture == 2)
					{
						vertices[i].weights.x -= w * YScale * DELTA;
						vertices[i].weights.y -= w * YScale * DELTA;
						vertices[i].weights.z += w * YScale * DELTA;
						vertices[i].weights.w -= w * YScale * DELTA;
					}
					if (brush[brushIdx].texture == 3)
					{
						vertices[i].weights.x -= w * YScale * DELTA;
						vertices[i].weights.y -= w * YScale * DELTA;
						vertices[i].weights.z -= w * YScale * DELTA;
						vertices[i].weights.w += w * YScale * DELTA;
					}
					NormalizeWeight(vertices[i].weights);
				}
				else if (heightBrush)
				{

					vertices[i].position.y += w * YScale * DELTA;

				}

			}
			terrain->mesh->Update();

		}




	}

	if (INPUT->KeyUp(VK_MBUTTON))
	{
		terrain->UpdateMeshNormal();
		terrain->DeleteStructuredBuffer();
		terrain->CreateStructuredBuffer();
	}

	////최종보스 문 열기
	//if (INPUT->KeyDown(VK_F1))
	//{
	//	dooropen = true;
	//}

	//if (dooropen == true)
	//{
	//	bossgate->rotation.x += 50.0f * TORADIAN * DELTA;
	//}

	////1번 벽 해제
	//if (INPUT->KeyDown('1'))
	//{
	//	unlock1 = true;
	//}

	//if (unlock1 == true)
	//{
	//	LockBlock->Find("Offset")->MoveWorldPos(-UP * 10.0f * DELTA);
	//	LockBlock->Find("BlockBox")->MoveWorldPos(-UP * 10.0f * DELTA);
	//}

	////2번 벽 해제
	//if (INPUT->KeyDown('2'))
	//{
	//	unlock2 = true;
	//}

	//if (unlock2 == true)
	//{
	//	LockBlock->Find("LockBlock1")->MoveWorldPos(-UP * 10.0f * DELTA);
	//}

	////3번 벽 해제
	//if (INPUT->KeyDown('3'))
	//{
	//	unlock3 = true;
	//}

	//if (unlock3 == true)
	//{
	//	LockBlock->Find("LockBlock2")->MoveWorldPos(-UP * 10.0f * DELTA);
	//}

	////기어 회전
	//if (INPUT->KeyDown('4'))
	//{
	//	gotrap = true;
	//}

	//if (gotrap == true)
	//{
	//	trap->visible = true;
	//	trap->rotation.z -= 50.0f * TORADIAN * DELTA;
	//	trap->MoveWorldPos(RIGHT * 60.0f * DELTA);
	//}

	//if (trap->GetWorldPos().x >= 900.0f)
	//{
	//	trap->SetLocalPosY(-1000.0f);
	//	trap->visible = false;
	//	gotrap = false;
	//}

	////던전 끝남
	//if (INPUT->KeyDown('5'))
	//{
	//	dungeonbreak = true;
	//}

	//if (dungeonbreak == true)
	//{
	//	dungeon->visible = false;
	//	dungeonend->visible = true;
	//}

	////톱니바퀴
	//saw->Find("SawTooth1")->rotation.z -= 120.0f * TORADIAN * DELTA;
	//saw->Find("SawTooth2")->rotation.z += 120.0f * TORADIAN * DELTA;
	//saw->Find("SawTooth3")->rotation.z -= 120.0f * TORADIAN * DELTA;
	//saw->Find("SawTooth4")->rotation.z += 120.0f * TORADIAN * DELTA;

	////1번톱니
	//if (INPUT->KeyDown('6'))
	//{
	//	saw->Find("SawTooth1")->SetLocalPosY(40.0f);
	//}

	//if (saw->Find("SawTooth1")->GetLocalPos().y == 40.0f)
	//{
	//	saw->Find("SawTooth1")->MoveLocalPos(RIGHT * SawSpeed * DELTA);
	//}

	//if (saw->Find("SawTooth1")->GetLocalPos().x >= 175.0f)
	//{
	//	saw->Find("SawTooth1")->SetLocalPosX(175.0f);
	//}

	////2번톱니
	//if (INPUT->KeyDown('7'))
	//{
	//	saw->Find("SawTooth2")->SetLocalPosY(40.0f);
	//}

	//if (saw->Find("SawTooth2")->GetLocalPos().y == 40.0f)
	//{
	//	saw->Find("SawTooth2")->MoveLocalPos(-RIGHT * SawSpeed * DELTA);
	//}

	//if (saw->Find("SawTooth2")->GetLocalPos().x <= -45.0f)
	//{
	//	saw->Find("SawTooth2")->SetLocalPosX(-45.0f);
	//}

	////3번톱니
	//if (INPUT->KeyDown('8'))
	//{
	//	saw->Find("SawTooth3")->SetLocalPosY(40.0f);
	//}

	//if (saw->Find("SawTooth3")->GetLocalPos().y == 40.0f)
	//{
	//	saw->Find("SawTooth3")->MoveLocalPos(RIGHT * SawSpeed * DELTA);
	//}

	//if (saw->Find("SawTooth3")->GetLocalPos().x >= 175.0f)
	//{
	//	saw->Find("SawTooth3")->SetLocalPosX(175.0f);
	//}

	////4번톱니
	//if (INPUT->KeyDown('9'))
	//{
	//	saw->Find("SawTooth4")->SetLocalPosY(40.0f);
	//}

	//if (saw->Find("SawTooth4")->GetLocalPos().y == 40.0f)
	//{
	//	saw->Find("SawTooth4")->MoveLocalPos(-RIGHT * SawSpeed * DELTA);
	//}

	//if (saw->Find("SawTooth4")->GetLocalPos().x <= -45.0f)
	//{
	//	saw->Find("SawTooth4")->SetLocalPosX(-45.0f);
	//}

	////떨어지는 돌
	//if (INPUT->KeyDown(VK_NUMPAD1))
	//{
	//	droprock1 = true;
	//}

	//if (droprock1 == true)
	//{
	//	droprock->Find("DropRock1")->visible = true;
	//	droprock->Find("DropRock1")->MoveWorldPos(-UP * RockSpeed * DELTA);
	//}

	//if (INPUT->KeyDown(VK_NUMPAD2))
	//{
	//	droprock2 = true;
	//}

	//if (droprock2 == true)
	//{
	//	droprock->Find("DropRock2")->visible = true;
	//	droprock->Find("DropRock2")->MoveWorldPos(-UP * RockSpeed * DELTA);
	//}

	//if (INPUT->KeyDown(VK_NUMPAD3))
	//{
	//	droprock3 = true;
	//}

	//if (droprock3 == true)
	//{
	//	droprock->Find("DropRock3")->visible = true;
	//	droprock->Find("DropRock3")->MoveWorldPos(-UP * RockSpeed * DELTA);
	//}

	////마지막 표창
	//if (INPUT->KeyDown(VK_NUMPAD4))
	//{
	//	shr->visible = true;
	//}

	//if (shr->visible == true)
	//{
	//	shr->rotation.z += 250.0f * TORADIAN * DELTA;
	//	shr->MoveWorldPos(FORWARD * 40.0f * DELTA);
	//}

	//if (shr->GetLocalPos().z >= 0.0f)
	//{
	//	shr->SetWorldPosY(-200.0f);
	//	shr->visible = false;
	//}
}
void Main::PreRender()
{
	LIGHT->Set();
}
void Main::Render()
{
	_brush.range = brush[brushIdx].range;
	_brush.shape = brush[brushIdx].shape;
	_brush.type = brush[brushIdx].type;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	D3D->GetDC()->Map(brushBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy_s(mappedResource.pData, sizeof(Brush), &_brush, sizeof(Brush));
	D3D->GetDC()->Unmap(brushBuffer, 0);

	Cam->Set();
	terrain->Render();
	/*sky->Render();
	dungeon->Render();
	dungeonend->Render();
	floor->Render();
	wall->Render();
	fieldwall->Render();
	LockBlock->Render();
	saw->Render();
	shr->Render();
	droprock->Render();
	bosswall->Render();
	bossgate->Render();
	trap->Render();*/
}

void Main::ResizeScreen()
{
	Cam->width = App.GetWidth();
	Cam->height = App.GetHeight();
	Cam->viewport.width = App.GetWidth();
	Cam->viewport.height = App.GetHeight();
}

void Main::NormalizeWeight(Vector4& in)
{
	float* weight = (float*)&in;
	float	sum = 0;
	for (int i = 0; i < 4; i++)
	{
		weight[i] = Util::Saturate(weight[i]);
		sum += weight[i];
	}
	for (int i = 0; i < 4; i++)
	{
		weight[i] /= sum;
	}
}

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE prevInstance, LPWSTR param, int command)
{
	App.SetAppName(L"Game1");
	App.SetInstance(instance);
    WIN->Create();
    D3D->Create();
	Main * main = new Main();
    main->Init();

	int wParam = (int)WIN->Run(main);


    main->Release();
	SafeDelete(main);
    D3D->DeleteSingleton();
	WIN->DeleteSingleton();

	return wParam;
}