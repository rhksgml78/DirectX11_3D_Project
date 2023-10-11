#pragma once
class CommandMove : public Command
{
private:
	Actor* actor;

	Vector3 dir;

	Vector3 targetPos;

	float moverange;
	float rength;
	float spd;
	float toTargetDis;
	float g;
	bool ismoving = false;
	bool isAttack;

	int randMove;
	int aniNum;
	bool moveanim = false;

private:

	Command_State Move();
public:
	CommandMove() {}
	CommandMove(Actor* actor)
		:actor(actor)
	{}
	~CommandMove();
	// Command을(를) 통해 상속됨
	virtual Command_State Excute() override;
	virtual void Update(shared_ptr<BlackBoard> board) override;
	virtual void LateUpdate(shared_ptr<BlackBoard> board) override;

	void SetSpd(float spd) { this->spd = spd; }
	void SetDir(Vector3 dir) { this->dir = dir; }

	void CheckState();
};

