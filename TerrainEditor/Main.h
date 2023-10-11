#pragma once

//메인에는 씬만 구성

struct Brush
{
	int		shape = 0;
	int		texture = 1;
	int     type = 0;
	float 	range = 10.0f;
	float	YScale = 3.0f;
};

class Main : public Scene
{
private:
	Camera* Cam;

	Terrain* terrain;

#pragma region KJM
	//Actor* dungeon;
	//Actor* dungeonend;
	//Actor* fieldwall;
	//Actor* wall;
	//Actor* bosswall;
	//Actor* bossgate;
	//Actor* floor;

	//Actor* LockBlock;
	//Actor* saw;
	//Actor* droprock;

	//Actor* trap;
	//Actor* shr;

	//Sky* sky;
#pragma endregion
	
	bool dooropen = false;
	bool unlock1 = false;
	bool unlock2 = false;
	bool unlock3 = false;

	bool gotrap = false;

	bool dungeonbreak = false;

	bool droprock1 = false;
	bool droprock2 = false;
	bool droprock3 = false;

	float SawSpeed = 90.0f;
	float RockSpeed = 90.0f;



	bool	heightBrush = true;
	bool	textureBrush = false;
	bool	nodeEdit = false;
	bool	objEdit = false;

	int		brushIdx;
	Brush	brush[3];
	Actor*	mousePoint;

	string file;

	struct Brush
	{
		Vector3 point;
		float	range = 2.0f;

		float	shape;
		float	type;
		Vector2 padding;
	} _brush;
	ID3D11Buffer* brushBuffer;

	int nodeState;
	int firstLink;
	int secondLink;

public:
	Main();
	~Main();
	virtual void Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void PreRender() override;
	virtual void ResizeScreen() override;
	void NormalizeWeight(Vector4& in);
};