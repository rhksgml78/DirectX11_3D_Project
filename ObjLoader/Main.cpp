#include "stdafx.h"
#include "Main.h"


Main::Main()
{

}

Main::~Main()
{

}

void Main::Init()
{
	actor = Actor::Create();
	//actor->LoadFile(".xml");

	//temp = Actor::Create();
	//temp->LoadFile(".xml");

	Cam = Camera::Create();
	Cam->LoadFile("Cam.xml");
	Camera::main = Cam;

	Grid = Actor::Create();
	Grid->LoadFile("Grid.xml");
	Camera::mainCamSpeed = 50.0f;
}

void Main::Release()
{
	RESOURCE->ReleaseAll();
	Cam->Release();
	Grid->Release();
	actor->Release();
	//temp->Release();
}


void Main::Update()
{
	ImGui::SliderFloat("Main_Cam_Speed",&Camera::mainCamSpeed,0.0f,100.0f);
	Camera::ControlMainCam();

	ImGui::Text("FPS: %d", TIMER->GetFramePerSecond());
	ImGui::Begin("Hierarchy");
	LIGHT->RenderDetail();
	Cam->RenderHierarchy();
	Grid->RenderHierarchy();
	for (size_t i = 0; i < List.size(); i++)
	{
		List[i]->RenderHierarchy();
	}
	actor->RenderHierarchy();
	//temp->RenderHierarchy();
	ImGui::End();

	ImGui::Begin("AssimpImporter");

	if (GUI->FileImGui("ModelImporter", "ModelImporter",
		".fbx,.obj,.x,.dae", "../Assets"))
	{
		file = ImGuiFileDialog::Instance()->GetCurrentFileName();
		string path = "../Assets/" + file;

		importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
		scene = importer.ReadFile
		(
			path,
			aiProcess_ConvertToLeftHanded
			| aiProcess_Triangulate
			| aiProcess_GenUVCoords
			| aiProcess_GenNormals
			| aiProcess_CalcTangentSpace
		);
		assert(scene != NULL and "Import Error");

		actor = Actor::Create(scene->mRootNode->mName.C_Str());
		actor->shader = RESOURCE->shaders.Load("4.cube.hlsl");
		actor->skeleton = new Skeleton();
		actor->boneIndex = 0;

		for (auto it = actor->obList.begin();
			it != actor->obList.end(); it++)
		{
			it->second->skelRoot = actor;
		}

		GameObject* offset = GameObject::Create("Offset");
		offset->shader = RESOURCE->shaders.Load("4.cube.hlsl");
		actor->AddBone(offset);

		ReadMaterial();

		ReadNode(offset, scene->mRootNode);
		{
			int tok = file.find_last_of(".");
			string checkPath = "../Contents/Skeleton/" + file.substr(0, tok);
			if (!PathFileExistsA(checkPath.c_str()))
			{
				CreateDirectoryA(checkPath.c_str(), NULL);
			}

			string filePath = file.substr(0, tok) + "/";
			actor->skeleton->file = filePath + file.substr(0, tok) + ".skel";
			actor->skeleton->SaveFile(actor->skeleton->file);
		}
		ReadMesh(offset, scene->mRootNode);


		List.push_back(actor);

		importer.FreeScene();


		actor->SaveFile("test.xml");
		actor->LoadFile("test.xml");

	}
	if (GUI->FileImGui("AnimationImporter", "AnimationImporter",
		".fbx,.obj,.x", "../Assets"))
	{
		file = ImGuiFileDialog::Instance()->GetCurrentFileName();
		string path = "../Assets/" + file;
		importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
		scene = importer.ReadFile
		(
			path,
			aiProcess_ConvertToLeftHanded
			| aiProcess_Triangulate
			| aiProcess_GenUVCoords
			| aiProcess_GenNormals
			| aiProcess_CalcTangentSpace
		);
		assert(scene != NULL and "Import Error");


		if (!actor->anim)
		{
			actor->anim = new Animations();
			//actor->animRoot = actor;
			/*for (auto it = actor->obList.begin();
				it != actor->obList.end(); it++)
			{
				it->second->animRoot = actor;
			}*/
		}



		//애니메이션 갯수
		for (UINT i = 0; i < scene->mNumAnimations; i++)
		{
			shared_ptr<Animation> Anim = make_shared<Animation>();
			aiAnimation* srcAnim = scene->mAnimations[i];
			size_t tok2 = file.find_last_of(".");
			Anim->file = file.substr(0, tok2) + to_string(i);
			Anim->frameMax = (int)srcAnim->mDuration + 1;
			Anim->tickPerSecond = srcAnim->mTicksPerSecond != 0.0 ? (float)srcAnim->mTicksPerSecond : 25.0f;
			Anim->boneMax = actor->boneIndexCount;
			Anim->arrFrameBone = new Matrix * [Anim->frameMax];
			for (UINT j = 0; j < Anim->frameMax; j++)
			{
				Anim->arrFrameBone[j] = new Matrix[actor->boneIndexCount];
			}




			//채널갯수 -> 본에 대응
			for (UINT j = 0; j < srcAnim->mNumChannels; j++)
			{
				AnimNode* animNode = new AnimNode();
				aiNodeAnim* srcAnimNode = srcAnim->mChannels[j];

				animNode->name = srcAnimNode->mNodeName.C_Str();
				//Scale
				for (UINT k = 0; k < srcAnimNode->mNumScalingKeys; k++)
				{
					AnimScale srcScale;
					srcScale.time = (float)srcAnimNode->mScalingKeys[k].mTime;
					srcScale.scale.x = (float)srcAnimNode->mScalingKeys[k].mValue.x;
					srcScale.scale.y = (float)srcAnimNode->mScalingKeys[k].mValue.y;
					srcScale.scale.z = (float)srcAnimNode->mScalingKeys[k].mValue.z;
					animNode->scale.push_back(srcScale);
				}
				//Position
				for (UINT k = 0; k < srcAnimNode->mNumPositionKeys; k++)
				{
					AnimPosition srcPosition;
					srcPosition.time = (float)srcAnimNode->mPositionKeys[k].mTime;
					srcPosition.pos.x = (float)srcAnimNode->mPositionKeys[k].mValue.x;
					srcPosition.pos.y = (float)srcAnimNode->mPositionKeys[k].mValue.y;
					srcPosition.pos.z = (float)srcAnimNode->mPositionKeys[k].mValue.z;
					animNode->position.push_back(srcPosition);
				}
				//Rotation
				for (UINT k = 0; k < srcAnimNode->mNumRotationKeys; k++)
				{
					AnimRotation srcRotation;
					srcRotation.time = (float)srcAnimNode->mRotationKeys[k].mTime;
					srcRotation.quater.x = (float)srcAnimNode->mRotationKeys[k].mValue.x;
					srcRotation.quater.y = (float)srcAnimNode->mRotationKeys[k].mValue.y;
					srcRotation.quater.z = (float)srcAnimNode->mRotationKeys[k].mValue.z;
					srcRotation.quater.w = (float)srcAnimNode->mRotationKeys[k].mValue.w;
					animNode->rotation.push_back(srcRotation);
				}

				GameObject* chanel = actor->Find(animNode->name);
				if (chanel)
				{
					Matrix S, R, T;
					Quaternion quter;
					Vector3 pos, scale;
					for (UINT k = 0; k < Anim->frameMax; k++)
					{
						pos = Interpolated::CalcInterpolatedPosition(animNode, (float)k, Anim->frameMax);
						scale = Interpolated::CalcInterpolatedScaling(animNode, (float)k, Anim->frameMax);
						quter = Interpolated::CalcInterpolatedRotation(animNode, (float)k, Anim->frameMax);

						S = Matrix::CreateScale(scale);
						R = Matrix::CreateFromQuaternion(quter);
						T = Matrix::CreateTranslation(pos);
						Matrix W = S * R * T;
						Anim->arrFrameBone[k][chanel->boneIndex] = chanel->GetLocal().Invert() * W;
					}
				}
				//여기서 채널끝(본)

			}
			//여기서 애님끝
			actor->anim->playList.push_back(Anim);

			{
				size_t tok = file.find_last_of(".");
				string checkPath = "../Contents/Animation/" + file.substr(0, tok);
				if (!PathFileExistsA(checkPath.c_str()))
				{
					CreateDirectoryA(checkPath.c_str(), NULL);
				}

				string filePath = file.substr(0, tok) + "/";
				Anim->file = filePath + Anim->file + ".anim";
				Anim->SaveFile(Anim->file);
			}

		}

		importer.FreeScene();
	}


	ImGui::End();

	Cam->Update();
	Grid->Update();
	for (size_t i = 0; i < List.size(); i++)
	{
		List[i]->Update();
	}
	actor->Update();
	//temp->Update();
}

void Main::LateUpdate()
{
	
}

void Main::Render()
{
	Cam->Set();
	Grid->Render();
	for (size_t i = 0; i < List.size(); i++)
	{
		List[i]->Render();
	}
	actor->Render();
	//temp->Render();
}
void Main::PreRender()
{
	LIGHT->Set();
}
void Main::ResizeScreen()
{
	Cam->width = App.GetWidth();
	Cam->height = App.GetHeight();
	Cam->viewport.width = App.GetWidth();
	Cam->viewport.height = App.GetHeight();
}

void Main::ReadMaterial()
{
	for (UINT i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* srcMtl = scene->mMaterials[i];
		Material* destMtl = new Material();
		aiColor3D tempColor;
		//이름 -키값
		destMtl->file = srcMtl->GetName().C_Str();
		//ambient
		srcMtl->Get(AI_MATKEY_COLOR_AMBIENT, tempColor);
		destMtl->ambient.x = tempColor.r;
		destMtl->ambient.y = tempColor.g;
		destMtl->ambient.z = tempColor.b;

		//diffuse
		srcMtl->Get(AI_MATKEY_COLOR_DIFFUSE, tempColor);
		destMtl->diffuse.x = tempColor.r;
		destMtl->diffuse.y = tempColor.g;
		destMtl->diffuse.z = tempColor.b;

		//specular
		srcMtl->Get(AI_MATKEY_COLOR_SPECULAR, tempColor);
		destMtl->specular.x = tempColor.r;
		destMtl->specular.y = tempColor.g;
		destMtl->specular.z = tempColor.b;

		//emissive
		srcMtl->Get(AI_MATKEY_COLOR_EMISSIVE, tempColor);
		destMtl->emissive.x = tempColor.r;
		destMtl->emissive.y = tempColor.g;
		destMtl->emissive.z = tempColor.b;

		//Shininess
		srcMtl->Get(AI_MATKEY_SHININESS, destMtl->shininess);
		//opacity
		srcMtl->Get(AI_MATKEY_OPACITY, destMtl->opacity);

		//Normal
		{
			aiString aifile;
			string TextureFile;
			aiReturn texFound;
			texFound = srcMtl->GetTexture(aiTextureType_NORMALS, 0, &aifile);
			TextureFile = aifile.C_Str();
			Util::Replace(&TextureFile, "\\", "/");
			size_t index = TextureFile.find_last_of('/');
			TextureFile = TextureFile.substr(index + 1, TextureFile.length());

			//텍스쳐가 있다.
			if (texFound == AI_SUCCESS && file != "")
			{
				destMtl->ambient.w = 1.0f;
				destMtl->normalMap = make_shared<Texture>();

				size_t tok = file.find_last_of(".");
				string checkPath = "../Contents/Texture/" + file.substr(0, tok);
				if (!PathFileExistsA(checkPath.c_str()))
				{
					CreateDirectoryA(checkPath.c_str(), NULL);
				}
				string orgin = "../Assets/" + TextureFile;
				string copy = "../Contents/Texture/" + file.substr(0, tok) + "/" + TextureFile;
				bool isCheck = true;
				CopyFileA(orgin.c_str(), copy.c_str(), isCheck);

				destMtl->normalMap->LoadFile(file.substr(0, tok) + "/" + TextureFile);

			}
		}

		//Diffuse
		{
			aiString aifile;
			string TextureFile;
			aiReturn texFound;
			texFound = srcMtl->GetTexture(aiTextureType_DIFFUSE, 0, &aifile);
			TextureFile = aifile.C_Str();
			Util::Replace(&TextureFile, "\\", "/");
			size_t index = TextureFile.find_last_of('/');
			TextureFile = TextureFile.substr(index + 1, TextureFile.length());

			//텍스쳐가 있다.
			if (texFound == AI_SUCCESS && file != "")
			{
				destMtl->ambient = Color(0.5, 0.5, 0.5);
				destMtl->diffuse.w = 1.0f;
				destMtl->diffuseMap = make_shared<Texture>();
				destMtl->ambient = Color{ 0.5, 0.5, 0.5 };

				size_t tok = file.find_last_of(".");
				string checkPath = "../Contents/Texture/" + file.substr(0, tok);
				if (!PathFileExistsA(checkPath.c_str()))
				{
					CreateDirectoryA(checkPath.c_str(), NULL);
				}
				string orgin = "../Assets/" + TextureFile;
				string copy = "../Contents/Texture/" + file.substr(0, tok) + "/" + TextureFile;
				bool isCheck = true;
				CopyFileA(orgin.c_str(), copy.c_str(), isCheck);

				destMtl->diffuseMap->LoadFile(file.substr(0, tok) + "/" + TextureFile);

			}
		}

		//specular
		{
			aiString aifile;
			string TextureFile;
			aiReturn texFound;
			texFound = srcMtl->GetTexture(aiTextureType_SPECULAR, 0, &aifile);
			TextureFile = aifile.C_Str();
			Util::Replace(&TextureFile, "\\", "/");
			size_t index = TextureFile.find_last_of('/');
			TextureFile = TextureFile.substr(index + 1, TextureFile.length());

			//텍스쳐가 있다.
			if (texFound == AI_SUCCESS && file != "")
			{
				destMtl->specular.w = 1.0f;
				destMtl->specularMap = make_shared<Texture>();

				size_t tok = file.find_last_of(".");
				string checkPath = "../Contents/Texture/" + file.substr(0, tok);
				if (!PathFileExistsA(checkPath.c_str()))
				{
					CreateDirectoryA(checkPath.c_str(), NULL);
				}
				string orgin = "../Assets/" + TextureFile;
				string copy = "../Contents/Texture/" + file.substr(0, tok) + "/" + TextureFile;
				bool isCheck = true;
				CopyFileA(orgin.c_str(), copy.c_str(), isCheck);

				destMtl->specularMap->LoadFile(file.substr(0, tok) + "/" + TextureFile);

			}
		}

		//emissive
		{
			aiString aifile;
			string TextureFile;
			aiReturn texFound;
			texFound = srcMtl->GetTexture(aiTextureType_EMISSIVE, 0, &aifile);
			TextureFile = aifile.C_Str();
			Util::Replace(&TextureFile, "\\", "/");
			size_t index = TextureFile.find_last_of('/');
			TextureFile = TextureFile.substr(index + 1, TextureFile.length());

			//텍스쳐가 있다.
			if (texFound == AI_SUCCESS && file != "")
			{
				destMtl->emissive.w = 1.0f;
				destMtl->emissiveMap = make_shared<Texture>();

				size_t tok = file.find_last_of(".");
				string checkPath = "../Contents/Texture/" + file.substr(0, tok);
				if (!PathFileExistsA(checkPath.c_str()))
				{
					CreateDirectoryA(checkPath.c_str(), NULL);
				}
				string orgin = "../Assets/" + TextureFile;
				string copy = "../Contents/Texture/" + file.substr(0, tok) + "/" + TextureFile;
				bool isCheck = true;
				CopyFileA(orgin.c_str(), copy.c_str(), isCheck);

				destMtl->emissiveMap->LoadFile(file.substr(0, tok) + "/" + TextureFile);

			}
		}

		size_t tok = file.find_last_of(".");
		string checkPath = "../Contents/Material/" + file.substr(0, tok);
		if (!PathFileExistsA(checkPath.c_str()))
		{
			CreateDirectoryA(checkPath.c_str(), NULL);
		}

		string filePath = file.substr(0, tok) + "/";
		destMtl->file = filePath + destMtl->file + ".mtl";
		destMtl->SaveFile(destMtl->file);
	}
}

void Main::ReadNode(GameObject* dest, aiNode* src)
{
	Matrix tempMat = ToMatrix(src->mTransformation);
	Vector3 s, r, t; Quaternion q;
	tempMat.Decompose(s, q, t);
	r = Util::QuaternionToYawPtichRoll(q);
	dest->scale = s; dest->rotation = r; dest->SetLocalPos(t);

	actor->Update();
	//로컬로 향하는 행렬값 넣기
	actor->skeleton->bonesOffset[dest->boneIndex] = dest->W.Invert();

	//자식노드들로 재귀호출
	for (UINT i = 0; i < src->mNumChildren; i++)
	{
		GameObject* temp =
			GameObject::Create(src->mChildren[i]->mName.C_Str());
		dest->AddBone(temp);
		//temp->mesh = RESOURCE->meshes.Load("2.Sphere.mesh");
		temp->shader = RESOURCE->shaders.Load("4.cube.hlsl");
		ReadNode(temp, src->mChildren[i]);
	}

}

void Main::ReadMesh(GameObject* dest, aiNode* src)
{

	for (UINT i = 0; i < src->mNumMeshes; i++)
	{
		UINT index = src->mMeshes[i];
		aiMesh* srcMesh = scene->mMeshes[index];
		vector<VertexModel>		VertexList;
		vector<VertexWeights>	VertexWeights;
		vector<UINT> indexList;
		string mtlFile = scene->mMaterials[srcMesh->mMaterialIndex]->GetName().C_Str();
		string meshFile = src->mName.C_Str();
		int tok = file.find_last_of(".");
		string filePath = file.substr(0, tok) + "/";
		mtlFile = filePath + mtlFile + ".mtl";

		//1:1 대응 배열
		VertexList.resize(srcMesh->mNumVertices);
		VertexWeights.resize(srcMesh->mNumVertices);


		ReadBoneData(srcMesh, VertexWeights);

		for (UINT j = 0; j < VertexList.size(); j++)
		{
			memcpy(&VertexList[j].position, &srcMesh->mVertices[j], sizeof(Vector3));
			if (srcMesh->HasTextureCoords(0))
				memcpy(&VertexList[j].uv, &srcMesh->mTextureCoords[0][j], sizeof(Vector2));
			if (srcMesh->HasNormals())
				memcpy(&VertexList[j].normal, &srcMesh->mNormals[j], sizeof(Vector3));
			if (srcMesh->HasTangentsAndBitangents())
				memcpy(&VertexList[j].tangent, &srcMesh->mTangents[j], sizeof(Vector3));

			//본데이터가 있을때
			if (!VertexWeights.empty())
			{
				VertexWeights[j].Normalize();

				VertexList[j].indices.x = (float)VertexWeights[j].boneIdx[0];
				VertexList[j].indices.y = (float)VertexWeights[j].boneIdx[1];
				VertexList[j].indices.z = (float)VertexWeights[j].boneIdx[2];
				VertexList[j].indices.w = (float)VertexWeights[j].boneIdx[3];

				VertexList[j].weights.x = VertexWeights[j].boneWeights[0];
				VertexList[j].weights.y = VertexWeights[j].boneWeights[1];
				VertexList[j].weights.z = VertexWeights[j].boneWeights[2];
				VertexList[j].weights.w = VertexWeights[j].boneWeights[3];
			}

		}//정점 저장 끝
		UINT IndexCount = srcMesh->mNumFaces;
		for (UINT j = 0; j < IndexCount; j++)
		{
			aiFace& face = srcMesh->mFaces[j];
			for (UINT k = 0; k < face.mNumIndices; k++)
			{
				indexList.emplace_back(face.mIndices[k]);
			}
		}
		VertexModel* VertexArray = new VertexModel[VertexList.size()];
		copy(VertexList.begin(), VertexList.end(), stdext::checked_array_iterator<VertexModel*>(VertexArray, VertexList.size()));

		UINT* IndexArray = new UINT[indexList.size()];
		copy(indexList.begin(), indexList.end(), stdext::checked_array_iterator<UINT*>(IndexArray, indexList.size()));

		if (i == 0)
		{
			dest->mesh = make_shared<Mesh>(VertexArray, VertexList.size(),
				IndexArray, indexList.size(), VertexType::MODEL);
			dest->material = new Material();
			dest->material->LoadFile(mtlFile);
			//dest->material = RESOURCE->materials.Load(mtlFile);

			{
				int tok = file.find_last_of(".");
				string checkPath = "../Contents/Mesh/" + file.substr(0, tok);
				if (!PathFileExistsA(checkPath.c_str()))
				{
					CreateDirectoryA(checkPath.c_str(), NULL);
				}

				string filePath = file.substr(0, tok) + "/";
				dest->mesh->file = filePath + meshFile + ".mesh";
				dest->mesh->SaveFile(dest->mesh->file);
			}

		}
		else
		{
			string Name = srcMesh->mName.C_Str() + string("MeshObject");
			GameObject* temp = GameObject::Create(Name);
			dest->AddBone(temp);
			temp->mesh = make_shared<Mesh>(VertexArray, VertexList.size(),
				IndexArray, indexList.size(), VertexType::MODEL);
			temp->shader = RESOURCE->shaders.Load("4.Cube.hlsl");
			temp->material = new Material();
			temp->material->LoadFile(mtlFile);
			//temp->material = RESOURCE->materials.Load(mtlFile);

			{
				int tok = file.find_last_of(".");
				string checkPath = "../Contents/Mesh/" + file.substr(0, tok);
				if (!PathFileExistsA(checkPath.c_str()))
				{
					CreateDirectoryA(checkPath.c_str(), NULL);
				}

				string filePath = file.substr(0, tok) + "/";
				temp->mesh->file = filePath + Name + ".mesh";
				temp->mesh->SaveFile(temp->mesh->file);
			}
		}
	}
	for (UINT i = 0; i < src->mNumChildren; i++)
	{
		ReadMesh(dest->children[src->mChildren[i]->mName.C_Str()],
			src->mChildren[i]);
	}
}

void Main::ReadBoneData(aiMesh* mesh, vector<VertexWeights>& vertexWeights)
{
	//메쉬가 가지고 있는 본 개수 만큼
	for (UINT i = 0; i < mesh->mNumBones; i++)
	{
		//현재본이 하이어라이키에서 몇번째 인덱스인가?
		string boneName = mesh->mBones[i]->mName.C_Str();
		int boneIndex = actor->Find(boneName)->boneIndex;
		for (UINT j = 0; j < mesh->mBones[i]->mNumWeights; j++)
		{
			UINT vertexID = mesh->mBones[i]->mWeights[j].mVertexId;
			vertexWeights[vertexID].AddData(boneIndex, mesh->mBones[i]->mWeights[j].mWeight);
		}
	}
}

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE prevInstance, LPWSTR param, int command)
{
	App.SetAppName(L"Game1");
	App.SetInstance(instance);
	WIN->Create();
	D3D->Create();
	Main* main = new Main();
	main->Init();
	int wParam = (int)WIN->Run(main);
	main->Release();
	SafeDelete(main);
	D3D->DeleteSingleton();
	WIN->DeleteSingleton();

	return wParam;
}


Vector3 Interpolated::CalcInterpolatedScaling(AnimNode* iter, float time, int Duration)
{
	//비어있으면 1,1,1반환
	if (iter->scale.empty())
		return Vector3(1.0f, 1.0f, 1.0f);
	//한개만 있는놈은 첫번째 값 반환
	if (iter->scale.size() == 1)
		return iter->scale.front().scale;
	//마지막 놈은 마지막값 반환
	if (time == Duration - 1)
	{
		return iter->scale.back().scale;
	}
	//보간 시작
	int scaling_index = FindScale(iter, time);
	//-1 인덱스가 없으므로 마지막 값으로 반환
	if (scaling_index == -1)
	{
		return iter->scale.back().scale;
	}
	//보간 끝
	UINT next_scaling_index = scaling_index + 1;
	assert(next_scaling_index < iter->scale.size());

	//차이 나는 시간값
	float delta_time = (float)(iter->scale[next_scaling_index].time
		- iter->scale[scaling_index].time);
	//보간값
	float factor = (time - (float)(iter->scale[scaling_index].time)) / delta_time;
	//얘도 하나만 있는애
	if (factor < 0.0f)
	{
		return iter->scale.front().scale;
	}

	auto start = iter->scale[scaling_index].scale;
	auto end = iter->scale[next_scaling_index].scale;

	start = Vector3::Lerp(start, end, factor);
	return start;
}
Quaternion Interpolated::CalcInterpolatedRotation(AnimNode* iter, float time, int Duration)
{
	//auto rot_frames = iter->second->rotation;

	if (iter->rotation.empty())
		return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);

	if (iter->rotation.size() == 1)
		return iter->rotation.front().quater;

	if (time == Duration - 1)
	{
		return iter->rotation.back().quater;
	}

	int quter_index = FindRot(iter, time);
	if (quter_index == -1)
	{
		return iter->rotation.back().quater;
	}
	UINT next_quter_index = quter_index + 1;
	assert(next_quter_index < iter->rotation.size());

	float delta_time = static_cast<float>(iter->rotation[next_quter_index].time - iter->rotation[quter_index].time);
	float factor = (time - static_cast<float>(iter->rotation[quter_index].time)) / delta_time;
	if (factor < 0.0f)
	{
		return iter->rotation.front().quater;
	}

	auto start = iter->rotation[quter_index].quater;
	auto end = iter->rotation[next_quter_index].quater;

	start = Quaternion::Slerp(start, end, factor);
	return start;
}
Vector3 Interpolated::CalcInterpolatedPosition(AnimNode* iter, float time, int Duration)
{
	//auto pos_frames = iter->second->position;

	if (iter->position.empty())
		return Vector3(0.0f, 0.0f, 0.0f);

	if (iter->position.size() == 1)
		return iter->position.front().pos;

	if (time == Duration - 1)
	{
		return iter->position.back().pos;
	}

	int position_index = FindPos(iter, time);

	if (position_index == -1)
	{
		return iter->position.back().pos;
	}

	UINT next_position_index = position_index + 1;
	assert(next_position_index < iter->position.size());

	float delta_time = static_cast<float>(iter->position[next_position_index].time - iter->position[position_index].time);
	float factor = (time - static_cast<float>(iter->position[position_index].time)) / delta_time;

	if (factor < 0.0f)
	{
		return iter->position.front().pos;
	}

	auto start = iter->position[position_index].pos;
	auto end = iter->position[next_position_index].pos;

	start = Vector3::Lerp(start, end, factor);
	return start;
}


int Interpolated::FindScale(AnimNode* iter, float time)
{
	//vector<AnimScale> scale_frames = iter->second->scale;

	if (iter->scale.empty())
		return -1;//-1 을 반환하면 오류로 터지게 됨

	for (UINT i = 0; i < iter->scale.size() - 1; i++)
	{
		if (time < (float)(iter->scale[i + 1].time))
			return i;
	}

	return -1;//-1 을 반환하면 오류로 터지게 됨
}
int Interpolated::FindRot(AnimNode* iter, float time)
{
	//auto pos_frames = iter->second->rotation;

	if (iter->rotation.empty())
		return -1;

	for (UINT i = 0; i < iter->rotation.size() - 1; i++)
	{
		if (time < static_cast<float>(iter->rotation[i + 1].time))
			return i;
	}

	return -1;
}
int Interpolated::FindPos(AnimNode* iter, float time)
{
	//auto pos_frames = iter->second->position;

	if (iter->position.empty())
		return -1;

	for (UINT i = 0; i < iter->position.size() - 1; i++)
	{
		if (time < static_cast<float>(iter->position[i + 1].time))
			return i;
	}

	return -1;
}