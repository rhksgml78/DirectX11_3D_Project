#include "framework.h"
void Actor::SaveFile(string file)
{
	this->file = file;
	Xml::XMLDocument* doc = new Xml::XMLDocument();
	Xml::XMLElement* ob = doc->NewElement("Root");
	doc->LinkEndChild(ob);
	if (skeleton and not parent)
	{
		Xml::XMLElement* Skeleton = doc->NewElement("Skeleton");
		ob->LinkEndChild(Skeleton);
		Skeleton->SetAttribute("File", skeleton->file.c_str());
		Skeleton->SetAttribute("BoneIndexCount", boneIndexCount);
	}
	if (anim)
	{
		Xml::XMLElement* Anim = doc->NewElement("Animations");
		ob->LinkEndChild(Anim);
		Anim->SetAttribute("AnimSize", (int)anim->playList.size());
		for (int i = 0; i < anim->playList.size(); i++)
		{
			string number = "File" + to_string(i);
			Anim->SetAttribute(number.c_str(), anim->playList[i]->file.c_str());
		}
	}


	SaveObject(ob, doc);
	string path = "../Contents/GameObject/" + file;
	doc->SaveFile(path.c_str());
	SafeDelete(doc);
}

void Actor::LoadFile(string file)
{
	this->file = file;
	Xml::XMLDocument* doc = new Xml::XMLDocument();
	string path = "../Contents/GameObject/" + file;
	Xml::XMLError result = doc->LoadFile(path.c_str());
	if (result != Xml::XML_SUCCESS) return;

	//������������
	ReleaseMember();

	Xml::XMLElement* ob;
	ob = doc->FirstChildElement();
	Xml::XMLElement* component;

	if ((component = ob->FirstChildElement("Skeleton")) and not parent)
	{
		file = component->Attribute("File");
		SafeDelete(skeleton);
		skeleton = new Skeleton();
		skeleton->LoadFile(file);
		boneIndexCount = component->IntAttribute("BoneIndexCount");


	}
	if (component = ob->FirstChildElement("Animations"))
	{
		SafeDelete(anim);
		anim = new Animations();

		int vecSize = component->IntAttribute("AnimSize");
		for (int i = 0; i < vecSize; i++)
		{
			string number = "File" + to_string(i);
			string aniFile = component->Attribute(number.c_str());
			anim->playList.push_back(RESOURCE->animations.Load(aniFile));
		}
	}
	name = ob->Attribute("Name");
	type = (ObType)ob->IntAttribute("ObType");
	obList[name] = this;
	LoadObject(ob);
	SafeDelete(doc);
}


void GameObject::SaveObject(Xml::XMLElement* This, Xml::XMLDocument* doc)
{
	This->SetAttribute("Name", name.c_str());
	This->SetAttribute("Visible", visible);
	This->SetAttribute("ObType", (int)type);

	if (root)
	{
		if (root->skeleton)
		{
			This->SetAttribute("BoneIndex", boneIndex);
		}
	}
	if (mesh)
	{
		Xml::XMLElement* Mesh = doc->NewElement("Mesh");
		This->LinkEndChild(Mesh);
		Mesh->SetAttribute("File", mesh->file.c_str());

		if (mesh->instanceBuffer)
		{
			Xml::XMLElement* Instance = doc->NewElement("Instance");
			This->LinkEndChild(Instance);
			mesh->SaveInstanceFile(mesh->instanceFile);
			Instance->SetAttribute("File", mesh->instanceFile.c_str());
		}

	}
	if (shader)
	{
		Xml::XMLElement* Shader = doc->NewElement("Shader");
		This->LinkEndChild(Shader);
		Shader->SetAttribute("File", shader->file.c_str());
	}
	if (material)
	{
		Xml::XMLElement* Material = doc->NewElement("Material");
		This->LinkEndChild(Material);
		Material->SetAttribute("File", material->file.c_str());
	}
	if (collider)
	{
		//Collider
		Xml::XMLElement* Collider = doc->NewElement("Collider");
		This->LinkEndChild(Collider);
		Collider->SetAttribute("ColliderType", (int)collider->type);
		Collider->SetAttribute("Visible", collider->visible);

		Xml::XMLElement* Position = doc->NewElement("Position");
		Collider->LinkEndChild(Position);
		Position->SetAttribute("X", collider->position.x);
		Position->SetAttribute("Y", collider->position.y);
		Position->SetAttribute("Z", collider->position.z);

		Xml::XMLElement* Scale = doc->NewElement("Scale");
		Collider->LinkEndChild(Scale);
		Scale->SetAttribute("X", collider->scale.x);
		Scale->SetAttribute("Y", collider->scale.y);
		Scale->SetAttribute("Z", collider->scale.z);

		Xml::XMLElement* Rotation = doc->NewElement("Rotation");
		Collider->LinkEndChild(Rotation);
		Rotation->SetAttribute("X", collider->rotation.x);
		Rotation->SetAttribute("Y", collider->rotation.y);
		Rotation->SetAttribute("Z", collider->rotation.z);
	}

	Transform::SaveTransform(This, doc);

	if (type == ObType::Camera)
	{
		Xml::XMLElement* Cam = doc->NewElement("Camera");
		This->LinkEndChild(Cam);
		Camera* CamOb = dynamic_cast<Camera*>(this);
		Cam->SetAttribute("Ortho", CamOb->ortho);
		Cam->SetAttribute("Fov", CamOb->fov);
		Cam->SetAttribute("Width", CamOb->width);
		Cam->SetAttribute("Height", CamOb->height);
		Cam->SetAttribute("NearZ", CamOb->nearZ);
		Cam->SetAttribute("FarZ", CamOb->farZ);
		Cam->SetAttribute("viewportX", CamOb->viewport.x);
		Cam->SetAttribute("viewportY", CamOb->viewport.y);
		Cam->SetAttribute("viewportW", CamOb->viewport.width);
		Cam->SetAttribute("viewportH", CamOb->viewport.height);
	}
	if (type == ObType::UI)
	{
		Xml::XMLElement* Ui = doc->NewElement("UI");
		This->LinkEndChild(Ui);
		UI* UiOb = dynamic_cast<UI*>(this);
		Ui->SetAttribute("PivotX", UiOb->pivot.x);
		Ui->SetAttribute("PivotY", UiOb->pivot.y);
	}
	if (type == ObType::Terrain)
	{
		Xml::XMLElement* terrain = doc->NewElement("terrain");
		This->LinkEndChild(terrain);
		Terrain* TerrainOb = dynamic_cast<Terrain*>(this);
		terrain->SetAttribute("uvScale", TerrainOb->uvScale);
		terrain->SetAttribute("triSize", TerrainOb->triSize);

		terrain->SetAttribute("diFile", TerrainOb->di.file.c_str());
		TerrainOb->di.SaveFile(TerrainOb->di.file.c_str());
	}
	else if (type == ObType::Billboard)
	{
		Xml::XMLElement* bill = doc->NewElement("Billboard");
		This->LinkEndChild(bill);
		Billboard* BillboardOb = dynamic_cast<Billboard*>(this);
		bill->SetAttribute("pivotX", BillboardOb->pivot.x);
		bill->SetAttribute("pivotY", BillboardOb->pivot.y);
	}
	else if (type == ObType::Light)
	{
		Xml::XMLElement* light = doc->NewElement("Light");
		This->LinkEndChild(light);
		Light* LightOb = dynamic_cast<Light*>(this);
		light->SetAttribute("colorR", LightOb->light->color.x);
		light->SetAttribute("colorG", LightOb->light->color.y);
		light->SetAttribute("colorB", LightOb->light->color.z);
		light->SetAttribute("directionX", LightOb->light->direction.x);
		light->SetAttribute("directionY", LightOb->light->direction.y);
		light->SetAttribute("directionZ", LightOb->light->direction.z);
		light->SetAttribute("inner", LightOb->light->inner);
		light->SetAttribute("outer", LightOb->light->outer);
		light->SetAttribute("isActive", LightOb->light->isActive);
		light->SetAttribute("lightType", LightOb->light->lightType);
		light->SetAttribute("positionX", LightOb->light->position.x);
		light->SetAttribute("positionY", LightOb->light->position.y);
		light->SetAttribute("positionZ", LightOb->light->position.z);
		light->SetAttribute("radius", LightOb->light->radius);
		light->SetAttribute("num", LightOb->light->size);
	}
	else if (type == ObType::Rain)
	{
		Xml::XMLElement* rain = doc->NewElement("Rain");
		This->LinkEndChild(rain);
		Rain* RainOb = dynamic_cast<Rain*>(this);
		rain->SetAttribute("particleScaleX", RainOb->particleScale.x);
		rain->SetAttribute("particleScaleY", RainOb->particleScale.y);
		rain->SetAttribute("particleCount", RainOb->particleCount);
		rain->SetAttribute("rangeX", RainOb->desc.range.x);
		rain->SetAttribute("rangeY", RainOb->desc.range.y);
		rain->SetAttribute("rangeZ", RainOb->desc.range.z);
		rain->SetAttribute("velocityX", RainOb->desc.velocity.x);
		rain->SetAttribute("velocityY", RainOb->desc.velocity.y);
		rain->SetAttribute("velocityZ", RainOb->desc.velocity.z);
		rain->SetAttribute("duration", RainOb->duration);

	}
	else if (type == ObType::Billboard)
	{
		Xml::XMLElement* bill = doc->NewElement("Billboard");
		This->LinkEndChild(bill);
		Billboard* BillboardOb = dynamic_cast<Billboard*>(this);
		bill->SetAttribute("pivotX", BillboardOb->pivot.x);
		bill->SetAttribute("pivotY", BillboardOb->pivot.y);
	}
	else if (type == ObType::Pop)
	{
		Xml::XMLElement* pop = doc->NewElement("Pop");
		This->LinkEndChild(pop);
		Pop* PopOb = dynamic_cast<Pop*>(this);
		pop->SetAttribute("particleScaleX", PopOb->particleScale.x);
		pop->SetAttribute("particleScaleY", PopOb->particleScale.y);
		pop->SetAttribute("particleCount", PopOb->particleCount);
		pop->SetAttribute("velocityScalar", PopOb->velocityScalar);
		pop->SetAttribute("duration", PopOb->desc.duration);
		pop->SetAttribute("gravity", PopOb->desc.gravity);
		pop->SetAttribute("duration", PopOb->duration);
	}




	Xml::XMLElement* Chidren = doc->NewElement("Children");
	This->LinkEndChild(Chidren);
	Chidren->SetAttribute("Size", (int)children.size());
	int i = 0;
	for (auto it = children.begin(); it != children.end(); it++)
	{
		string temp = "Child" + to_string(i++);
		Xml::XMLElement* Child = doc->NewElement(temp.c_str());
		This->LinkEndChild(Child);
		it->second->SaveObject(Child, doc);
	}
}

void GameObject::LoadObject(Xml::XMLElement* This)
{
	Xml::XMLElement* component;
	string file;
	visible = This->BoolAttribute("Visible");

	if (root)
	{
		if (root->skeleton)
		{
			boneIndex = This->IntAttribute("BoneIndex");

			skelRoot = root;

		}
	}
	if (component = This->FirstChildElement("Mesh"))
	{
		file = component->Attribute("File");
		if (file.size() > 0)
		{
			SafeReset(mesh);
			mesh = RESOURCE->meshes.Load(file);

			Xml::XMLElement* component2;
			if (component2 = This->FirstChildElement("Instance"))
			{
				mesh->instanceFile = component2->Attribute("File");
				mesh->LoadInstanceFile(mesh->instanceFile);
			}
		}

	}
	if (component = This->FirstChildElement("Shader"))
	{
		file = component->Attribute("File");
		SafeReset(shader);
		shader = RESOURCE->shaders.Load(file);
	}
	if (component = This->FirstChildElement("Material"))
	{
		file = component->Attribute("File");
		SafeDelete(material);
		material = new Material();
		material->LoadFile(file);
	}

	if (component = This->FirstChildElement("Collider"))
	{
		int coltype;
		Xml::XMLElement* transform;
		coltype = component->IntAttribute("ColliderType");
		SafeDelete(collider);
		collider = new Collider((ColliderType)coltype);
		collider->visible = component->BoolAttribute("Visible");
		transform = component->FirstChildElement("Position");
		collider->position.x = transform->FloatAttribute("X");
		collider->position.y = transform->FloatAttribute("Y");
		collider->position.z = transform->FloatAttribute("Z");

		transform = component->FirstChildElement("Scale");
		collider->scale.x = transform->FloatAttribute("X");
		collider->scale.y = transform->FloatAttribute("Y");
		collider->scale.z = transform->FloatAttribute("Z");

		transform = component->FirstChildElement("Rotation");
		collider->rotation.x = transform->FloatAttribute("X");
		collider->rotation.y = transform->FloatAttribute("Y");
		collider->rotation.z = transform->FloatAttribute("Z");
	}

	if (type == ObType::Camera)
	{
		Camera* CamOb = dynamic_cast<Camera*>(this);
		component = This->FirstChildElement("Camera");

		CamOb->ortho = component->BoolAttribute("Ortho");
		CamOb->fov = component->FloatAttribute("Fov");
		CamOb->width = component->FloatAttribute("Width");
		CamOb->height = component->FloatAttribute("Height");
		CamOb->nearZ = component->FloatAttribute("NearZ");
		CamOb->farZ = component->FloatAttribute("FarZ");
		CamOb->viewport.x = component->FloatAttribute("viewportX");
		CamOb->viewport.y = component->FloatAttribute("viewportY");
		CamOb->viewport.width = component->FloatAttribute("viewportW");
		CamOb->viewport.height = component->FloatAttribute("viewportH");

	}
	if (type == ObType::UI)
	{
		UI* UiOb = dynamic_cast<UI*>(this);
		component = This->FirstChildElement("UI");
		UiOb->pivot.x = component->FloatAttribute("PivotX");
		UiOb->pivot.y = component->FloatAttribute("PivotY");
	}
	else if (type == ObType::Billboard)
	{
		Billboard* BillboardOb = dynamic_cast<Billboard*>(this);
		component = This->FirstChildElement("Billboard");
		BillboardOb->pivot.x = component->FloatAttribute("pivotX");
		BillboardOb->pivot.y = component->FloatAttribute("pivotY");
	}
	if (type == ObType::Terrain)
	{
		Terrain* TerrainOb = dynamic_cast<Terrain*>(this);
		component = This->FirstChildElement("terrain");
		TerrainOb->uvScale = component->FloatAttribute("uvScale");
		TerrainOb->triSize = component->IntAttribute("triSize");

		TerrainOb->di.file = component->Attribute("diFile");

		TerrainOb->di.LoadFile(TerrainOb->di.file);
	}
	else if (type == ObType::Light)
	{
		Light* LightOb = dynamic_cast<Light*>(this);
		component = This->FirstChildElement("Light");

		LightOb->light->color.x = component->FloatAttribute("colorR");
		LightOb->light->color.y = component->FloatAttribute("colorG");
		LightOb->light->color.z = component->FloatAttribute("colorB");
		LightOb->light->direction.x = component->FloatAttribute("directionX");
		LightOb->light->direction.y = component->FloatAttribute("directionY");
		LightOb->light->direction.z = component->FloatAttribute("directionZ");
		LightOb->light->inner = component->FloatAttribute("inner");
		LightOb->light->outer = component->FloatAttribute("outer");
		LightOb->light->isActive = component->IntAttribute("isActive");
		LightOb->light->lightType = component->IntAttribute("lightType");
		LightOb->light->position.x = component->FloatAttribute("positionX");
		LightOb->light->position.y = component->FloatAttribute("positionY");
		LightOb->light->position.z = component->FloatAttribute("positionZ");
		LightOb->light->radius = component->FloatAttribute("radius");
		LightOb->light->size = component->IntAttribute("num");
	}
	else if (type == ObType::Billboard)
	{
		Billboard* BillboardOb = dynamic_cast<Billboard*>(this);
		component = This->FirstChildElement("Billboard");
		BillboardOb->pivot.x = component->FloatAttribute("pivotX");
		BillboardOb->pivot.y = component->FloatAttribute("pivotY");
	}
	else if (type == ObType::Rain)
	{
		Rain* RainOb = dynamic_cast<Rain*>(this);
		component = This->FirstChildElement("Rain");

		RainOb->particleScale.x = component->FloatAttribute("particleScaleX");
		RainOb->particleScale.y = component->FloatAttribute("particleScaleY");
		RainOb->particleCount = component->IntAttribute("particleCount");
		RainOb->desc.range.x = component->FloatAttribute("rangeX");
		RainOb->desc.range.y = component->FloatAttribute("rangeY");
		RainOb->desc.range.z = component->FloatAttribute("rangeZ");
		RainOb->desc.velocity.x = component->FloatAttribute("velocityX");
		RainOb->desc.velocity.y = component->FloatAttribute("velocityY");
		RainOb->desc.velocity.z = component->FloatAttribute("velocityZ");
		RainOb->duration = component->FloatAttribute("duration");
		RainOb->Reset();
	}
	else if (type == ObType::Pop)
	{
		Pop* PopOb = dynamic_cast<Pop*>(this);
		component = This->FirstChildElement("Pop");
		PopOb->particleScale.x = component->FloatAttribute("particleScaleX");
		PopOb->particleScale.y = component->FloatAttribute("particleScaleY");
		PopOb->particleCount = component->IntAttribute("particleCount");
		PopOb->velocityScalar = component->FloatAttribute("velocityScalar");
		PopOb->desc.duration = component->FloatAttribute("duration");
		PopOb->desc.gravity = component->FloatAttribute("gravity");
		PopOb->duration = component->FloatAttribute("duration");
		PopOb->Reset();
	}
	Transform::LoadTransform(This);

	component = This->FirstChildElement("Children");
	int size = component->IntAttribute("Size");

	for (int i = 0; i != size; i++)
	{
		string Tag = "Child" + to_string(i);
		Xml::XMLElement* ob = This->FirstChildElement(Tag.c_str());
		string childName = ob->Attribute("Name");
		ObType Type = (ObType)ob->IntAttribute("ObType");
		if (Type == ObType::GameObject)
		{
			GameObject* temp = GameObject::Create(childName);
			AddChild(temp);
			temp->LoadObject(ob);
		}
		else if (Type == ObType::Actor)
		{
			Actor* temp = Actor::Create(childName);
			AddChild(temp);
			temp->LoadObject(ob);
		}
		else if (Type == ObType::Camera)
		{
			Camera* temp = Camera::Create(childName);
			AddChild(temp);
			temp->LoadObject(ob);
		}
		else if (Type == ObType::UI)
		{
			UI* temp = UI::Create(childName);
			AddChild(temp);
			temp->LoadObject(ob);
		}
		else if (Type == ObType::Billboard)
		{
			Billboard* temp = Billboard::Create(childName);
			AddChild(temp);
			temp->LoadObject(ob);
		}
		else if (Type == ObType::Light)
		{
			Light* temp = Light::Create(childName);
			if (temp)
			{
				AddChild(temp);
				temp->LoadObject(ob);
			}
		}
		else if (Type == ObType::Rain)
		{
			Rain* temp = Rain::Create(childName);
			AddChild(temp);
			temp->LoadObject(ob);
		}
		else if (Type == ObType::Pop)
		{
			Pop* temp = Pop::Create(childName);
			AddChild(temp);
			temp->LoadObject(ob);
		}
	}
}
void GameObject::CopyChild(GameObject* src)
{
	//���� �̹� ��������
	for (auto it = src->children.begin(); it != src->children.end(); it++)
	{
		GameObject* Child = nullptr;
		GameObject* srcChild = it->second;
		//Child ����
		if (it->second->type == ObType::GameObject)
		{
			Child = new GameObject(*srcChild);
		}
		if (it->second->type == ObType::Actor)
		{
			Actor* src = (Actor*)srcChild;
			Child = new Actor(*src);
		}
		if (it->second->type == ObType::Camera)
		{
			Camera* src = (Camera*)srcChild;
			Child = new Camera(*src);
		}
		if (it->second->type == ObType::Light)
		{
			Light* src = (Light*)srcChild;
			Child = new Light(*src);
		}
		if (it->second->type == ObType::Terrain)
		{
			Terrain* src = (Terrain*)srcChild;
			Child = new Terrain(*src);
		}
		if (it->second->type == ObType::UI)
		{
			UI* src = (UI*)srcChild;
			Child = new UI(*src);
		}
		if (it->second->type == ObType::Billboard)
		{
			Billboard* src = (Billboard*)srcChild;
			Child = new Billboard(*src);
		}

		if (it->second->type == ObType::Rain)
		{
			Rain* src = (Rain*)srcChild;
			Child = new Rain(*src);
		}
		if (it->second->type == ObType::Pop)
		{
			Pop* src = (Pop*)srcChild;
			Child = new Pop(*src);
		}
		//�ּҿ���
		Child->root = this->root;
		Child->skelRoot = this->root;
		Child->parent = this;
		children[Child->name] = Child;
		//���̺���
		this->AddChild(Child);
		//�ڽ��� ���ȣ��
		Child->CopyChild(srcChild);
	}
}
void Transform::SaveTransform(Xml::XMLElement* This, Xml::XMLDocument* doc)
{
	Xml::XMLElement* Trans = doc->NewElement("Transform");
	Xml::XMLElement* Pos = doc->NewElement("Position");
	Xml::XMLElement* Scale = doc->NewElement("Scale");
	Xml::XMLElement* Rot = doc->NewElement("Rotation");
	This->LinkEndChild(Trans);
	Trans->LinkEndChild(Pos);
	Trans->LinkEndChild(Scale);
	Trans->LinkEndChild(Rot);
	Pos->SetAttribute("X", position.x);
	Pos->SetAttribute("Y", position.y);
	Pos->SetAttribute("Z", position.z);
	Scale->SetAttribute("X", scale.x);
	Scale->SetAttribute("Y", scale.y);
	Scale->SetAttribute("Z", scale.z);
	Rot->SetAttribute("X", rotation.x);
	Rot->SetAttribute("Y", rotation.y);
	Rot->SetAttribute("Z", rotation.z);
}

void Transform::LoadTransform(Xml::XMLElement* This)
{
	Xml::XMLElement* component;
	Xml::XMLElement* transform;
	component = This->FirstChildElement("Transform");
	transform = component->FirstChildElement("Position");
	position.x = transform->FloatAttribute("X");
	position.y = transform->FloatAttribute("Y");
	position.z = transform->FloatAttribute("Z");
	transform = component->FirstChildElement("Scale");
	scale.x = transform->FloatAttribute("X");
	scale.y = transform->FloatAttribute("Y");
	scale.z = transform->FloatAttribute("Z");
	transform = component->FirstChildElement("Rotation");
	rotation.x = transform->FloatAttribute("X");
	rotation.y = transform->FloatAttribute("Y");
	rotation.z = transform->FloatAttribute("Z");
}

