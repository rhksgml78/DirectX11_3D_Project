#include "framework.h"

GameObject::GameObject()
{
	type = ObType::GameObject;
	root = nullptr;
	visible = true;
	mesh = nullptr;
	shader = nullptr;
	material = nullptr;
	collider = nullptr;
	skelRoot = nullptr;
	rootMotion = false;
}
GameObject::GameObject(GameObject& src)
{


	//여기는 동적할당
	if (src.collider)
		collider = new Collider(*src.collider);
	else
		collider = nullptr;

	if (src.material)
		material = new Material(*src.material);
	else
		material = nullptr;

	//대입 (포인터는 얕은복사)
	mesh = src.mesh;
	shader = src.shader;
	type = src.type;
	name = src.name;
	position = src.position;
	scale = src.scale;
	visible = src.visible;
	rotation = src.rotation;
	boneIndex = src.boneIndex;
}
Actor::Actor()
{
	type = ObType::Actor;
	root = this;
	skeleton = nullptr;
	anim = nullptr;
}
Actor::Actor(Actor& src)
{
	root = this;
	parent = nullptr;
	obList[src.name] = this;
	type = ObType::Actor;
	file = src.file;
	boneIndexCount = src.boneIndexCount;
	if (src.collider)
		collider = new Collider(*src.collider);
	else
		collider = nullptr;
	if (src.material)
		material = new Material(*src.material);
	else
		material = nullptr;
	if (src.skeleton)
		skeleton = new Skeleton(*src.skeleton);
	else
		skeleton = nullptr;
	if (src.anim)
		anim = new Animations(*src.anim);
	else
		anim = nullptr;
}
GameObject::~GameObject()
{
	mesh.reset();
	shader.reset();
	SafeDelete(collider);
	SafeDelete(material);
}
Actor::~Actor()
{
	SafeDelete(anim);
	SafeDelete(skeleton);
}





GameObject* GameObject::Create(string name)
{
	GameObject* temp = new GameObject();
	temp->name = name;
	return temp;
}

void GameObject::Release()
{
	for (auto it = children.begin(); it != children.end(); it++)
	{
		SafeRelease(it->second);
	}
	delete this;
}


void Actor::Release()
{
	for (auto it = children.begin(); it != children.end(); it++)
	{
		SafeRelease(it->second);
	}
	delete this;
}

void Actor::ReleaseMember()
{
	for (auto it = children.begin(); it != children.end(); it++)
	{
		it->second->Release();
	}

	obList.clear();
	children.clear();
	SafeDelete(anim);
	SafeDelete(skeleton);
	SafeDelete(material);
	SafeDelete(collider);
	SafeReset(shader);
}

Actor* Actor::Create(string name)
{
	Actor* temp = new Actor();
	temp->name = name;
	return temp;
}

Actor* Actor::Create(Actor* src)
{
	Actor* temp = new Actor(*src);
	temp->CopyChild(src);
	return temp;
}


void GameObject::Update()
{

	if (skelRoot and boneIndex != -1)
	{
		if (skelRoot->anim)
		{
			if (rootMotion)
			{
				Matrix temp = skelRoot->anim->GetFrameBone(boneIndex);
				temp._41 = 0.0f;
				temp._42 = 0.0f;
				temp._43 = 0.0f;
				Transform::UpdateAnim(std::move(temp));
			}
			else
			{
				Transform::UpdateAnim(skelRoot->anim->GetFrameBone(boneIndex));
			}
		}
		else
		{
			Transform::Update();
		}
		Matrix temp = skelRoot->skeleton->bonesOffset[boneIndex] * W;
		//행우선->열우선
		skelRoot->skeleton->bones[boneIndex] = temp.Transpose();
	}
	else
	{
		Transform::Update();
	}
	if (collider)
		collider->Update(this);

	for (auto it = children.begin(); it != children.end(); it++)
		it->second->Update();
}

void GameObject::Render()
{
	if (visible)
	{
		//내가 메쉬가 있을떄 렌더링 시작
		if (mesh and shader)
		{
			mesh->Set();
			Transform::Set();
			shader->Set();

			if (material)
				material->Set();
			else
				defalutMaterial->Set();

			// 인스턴싱 적용전 드로우콜
			//D3D->GetDC()->DrawIndexed(mesh->indexCount, 0, 0);
			
			// 인스턴싱 적용후 변경된 드로우콜
			mesh->DrawCall();
		}

		for (auto it = children.begin(); it != children.end(); it++)
			it->second->Render();
	}


	if (collider)
		collider->Render();



}
void GameObject::Render(shared_ptr<Shader> shader)
{
	if (visible)
	{
		//내가 메쉬가 있을떄 렌더링 시작
		if (mesh and shader)
		{
			mesh->Set();
			Transform::Set();
			shader->Set();

			if (material)
				material->Set();
			else
				defalutMaterial->Set();

			// 인스턴싱 적용전 드로우콜
			//D3D->GetDC()->DrawIndexed(mesh->indexCount, 0, 0);
		
			// 인스턴싱 적용후 변경된 드로우콜
			mesh->DrawCall();
		}

		for (auto it = children.begin(); it != children.end(); it++)
			it->second->Render(shader);
	}


	if (collider)
		collider->Render();

}
GameObject* GameObject::axis = nullptr;
Material* GameObject::defalutMaterial = nullptr;
void GameObject::CreateStaticMember()
{
	axis = new GameObject();
	axis->mesh = make_shared<Mesh>();
	axis->shader = make_shared<Shader>();
	axis->mesh->LoadFile("1.Transform.mesh");
	axis->shader->LoadFile("1.Cube.hlsl");
	defalutMaterial = new Material();
}

void GameObject::DeleteStaticMember()
{
	SafeDelete(axis);
	SafeDelete(defalutMaterial);
}

void GameObject::RenderAxis()
{
	if (not GUI->target)return;
	GameObject* target = GUI->target;
	axis->S = Matrix::CreateScale(App.GetHeight() * 0.1f);
	if (not target->parent)
		axis->W = axis->S * target->T;
	else if (worldPos)
		axis->W = axis->S * Matrix::CreateTranslation(target->GetWorldPos());
	else
		axis->W = axis->S * target->T * target->parent->W;

	axis->Render();
}

void GameObject::AddChild(GameObject* child)
{
	if (root->Find(child->name))
		return;
	root->obList[child->name] = child;
	children[child->name] = child;
	child->parent = this;
	child->root = root;

}

void GameObject::AddBone(GameObject* child)
{
	if (root->Find(child->name))
		return;
	root->obList[child->name] = child;
	children[child->name] = child;
	child->parent = this;
	child->root = root;
	child->boneIndex = root->boneIndexCount;
	root->boneIndexCount++;
}

GameObject* Actor::Find(string name)
{
	auto it = obList.find(name);
	//찾았다
	if (it != obList.end())
	{
		return it->second;
	}
	return nullptr;
}

bool Actor::DeleteObject(string Name)
{
	if (Name == name) return false;

	auto temp = obList.find(Name);

	if (temp == obList.end())  return false;

	//지워질대상,지워질대상의 부모 포인터
	GameObject* Target = temp->second;
	GameObject* Parent = temp->second->parent;
	//부모리스트에서 타겟 비우기
	Parent->children.erase(Parent->children.find(Name));

	for (auto it = Target->children.begin();
		it != Target->children.end(); it++)
	{
		Parent->children[it->second->name] = it->second;
		it->second->parent = Parent;
		//SafeRelease(it->second);
	}
	SafeDelete(Target);
	obList.erase(temp);
	return true;
}

bool Actor::DeleteAllObject(string Name)
{
	if (Name == name) return false;

	auto temp = obList.find(Name);

	if (temp == obList.end())  return false;

	//지워질대상,지워질대상의 부모 포인터
	GameObject* Target = temp->second;
	GameObject* Parent = temp->second->parent;
	//부모리스트에서 타겟 비우기
	Parent->children.erase(Parent->children.find(Name));
	root->obList.erase(root->obList.find(Name));
	while (1)
	{
		if (Target->children.empty())break;
		auto it = Target->children.begin();
		DeleteAllObject(it->first);
	}
	SafeDelete(Target);
	return true;
}

void Actor::Update()
{

	if (anim)
	{
		anim->Update();
	}

	GameObject::Update();
}

void Actor::Render()
{

	if (skeleton)
	{
		skeleton->Set();
	}
	GameObject::Render();
}

void Actor::Render(shared_ptr<Shader> shader)
{
	if (skeleton)
	{
		skeleton->Set();
	}
	GameObject::Render(shader);
}
