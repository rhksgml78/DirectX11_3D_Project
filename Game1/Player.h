#pragma once

enum class PlayerState
{
	IDLE,
	WALK,
	BACKWALK,
	ATTACK,
	ENDINGMOVE,
	ENDINGROAR,
	DIE,
	JUMP
};

class Player
{
	Camera* Cam;
	Actor* dead;

	//Vector3 down;
	float	gravity;

public:
	bool iscontrol;
	float	speed;
	Vector3 inputDir;
	PlayerState state;
	bool isLanding;
	Vector3 lastPos;
	Actor* root;

	void Init();
	void Update();
	void Hi();
	void Render();
	void WASD();
	Ray GetRay() {
		Ray Top;
		Top.position = root->GetWorldPos() + Vector3(0, 100, 0);
		Top.direction = Vector3(0, -1, 0);
		return Top;
	}
	Ray GetLowRay() {
		Ray Top;
		Top.position = root->GetWorldPos() + Vector3(0, 2, 0);
		Top.direction = Vector3(0, -1, 0);
		return Top;
	}
	void OutOfRange();
	void OnFloor(Vector3 Hit);
	void BackMove() { root->SetWorldPos(lastPos); root->Update(); };
};

