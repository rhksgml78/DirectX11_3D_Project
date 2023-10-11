#pragma once

enum class DeadState
{
	IDLE,
	WALK,
	JUMP
};

class Deadpool
{
	Camera* Cam;
	Actor* dead;
	


	DeadState state;


	Vector3 inputDir;
	float	speed;

	//Vector3 down;
	float	gravity;

public:
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
		Top.position = root->GetWorldPos() + Vector3(0, 1000, 0);
		Top.direction = Vector3(0, -1, 0);
		return Top;
	}
	void OutOfRange();
	void OnFloor(Vector3 Hit);
	void BackMove() { root->SetWorldPos(lastPos); root->Update(); };
};

