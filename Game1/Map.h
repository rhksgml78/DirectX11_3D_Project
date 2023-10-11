#pragma once
class Map : public Scene
{

private:
	class APlayer* player;

public:
	Terrain* terrain;

	Actor* dungeon;
	Actor* dungeonend;
	Actor* fieldwall;
	Actor* wall;
	Actor* bosswall;
	Actor* bossgate;
	Actor* floor;

	Actor* LockBlock;
	Actor* saw;
	Actor* droprock;

	Actor* trap;
	Actor* shr;

	Sky* sky;

	bool dooropen = false;
	bool unlock1 = false;
	bool unlock2 = false;
	bool unlock3 = false;

	bool gotrap = false;

	bool dungeonbreak = false;

	bool droprock1 = false;
	bool droprock2 = false;
	bool droprock3 = false;

	bool GoBoss = false;

	float SawSpeed = 90.0f;
	float RockSpeed = 90.0f;

	bool BossPattern = false;

	Map();
	~Map();
	void Init();
	void Release();
	void Update();
	void LateUpdate();
	void RenderHierarchy();
	void Render();
	void PreRender();
	void ResizeScreen();
};

