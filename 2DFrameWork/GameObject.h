#pragma once

enum class ObType
{
	GameObject,
	Actor,
	Camera,
	Terrain,
	UI,
	Billboard,
	Light,
	Rain,
	Pop
};

class GameObject : public Transform
{
	friend class Actor;
	//static
private:
	static GameObject* axis;
	static Material* defalutMaterial;

public:
	static GameObject* Create(string name = "GameObject");
	static void CreateStaticMember();
	static void DeleteStaticMember();
	static void RenderAxis();
	//member
protected:
	ObType						type;

public:
	shared_ptr<Shader>			shader;
	shared_ptr<Mesh>			mesh;
	string						name;	//key
	bool						visible;
	class Material*				material;
	class Collider*				collider;
	class Actor*				root;
	class Actor*				skelRoot;
	map<string, GameObject*>	children;
	int                         boneIndex = -1;
	bool						rootMotion;

	//Method
protected:
	GameObject();
	GameObject(GameObject& src);
	virtual	~GameObject();
	void	SaveObject(Xml::XMLElement* This, Xml::XMLDocument* doc);
	void	LoadObject(Xml::XMLElement* This);
	virtual void        CopyChild(GameObject* src);
public:
	virtual void	Release();
	virtual void	Update();
	virtual void	Render();
	virtual void	Render(shared_ptr<Shader> shader);
	void			AddChild(GameObject* child);
	void			AddBone(GameObject* child);
	virtual bool	RenderHierarchy();
	virtual void	RenderDetail();
	bool            Intersect(GameObject* target);
	bool            Intersect(Ray Ray, Vector3& Hit);
	//Getter Setter
};




class Actor : public GameObject
{
	friend GameObject;
public:
	static Actor* Create(string name = "Actor");
	static Actor* Create(Actor* src);

private:

public:
	enum ActorType
	{
		PLAYER,
		BOSS,
		MONSTER,
		MONSTER2,
		NPC
	}actortype;

	unordered_map<string, GameObject*> obList;
	string			file;
	Skeleton* skeleton;
	Animations* anim;
	int             boneIndexCount = 1;
protected:
	Actor();
	Actor(Actor& src);
	virtual	~Actor();
public:
	virtual void	Release();
	void			ReleaseMember();
	GameObject*		Find(string name);
	bool            DeleteObject(string Name);
	bool            DeleteAllObject(string Name);
	void			SaveFile(string file);	
	void			LoadFile(string file);
	virtual void	RenderDetail();
	virtual void	Update();
	virtual void    Render();
	virtual void    Render(shared_ptr<Shader> shader);
	//void			RotationZ() { rotation.z += 0.03f; }
};

