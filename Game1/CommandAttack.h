#pragma once
class CommandAttack : public Command
{
private:
	Actor* actor;

	Vector3 dir;

	Vector3 targetPos;

	int animNum = -1;
	int currentAnim = -1;
	//int currentNum = -1;

	float atkTime = 0.0f;

	float atkRange;

	float range;

	bool isAttack;

	// 공격이 끝났을타이밍에 플레이어방향 바라보도록 조절
	float g; // 타이머용 변수
	float rotY; // 회전용 변수

	Command_State Attack();
public:
	CommandAttack() {}
	CommandAttack(Actor* actor)
		:actor(actor)
	{}
	~CommandAttack();
	// Command을(를) 통해 상속됨
	virtual Command_State Excute() override;

public:
	void SetActor(Actor* actor) { this->actor = actor; }
	void SetAnim(int anim) { animNum = anim; }

public:

	virtual void Update(shared_ptr<BlackBoard> board) override;
	virtual void LateUpdate(shared_ptr<BlackBoard> board) override;
};

