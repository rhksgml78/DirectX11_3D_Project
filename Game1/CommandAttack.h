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

	// ������ ������Ÿ�ֿ̹� �÷��̾���� �ٶ󺸵��� ����
	float g; // Ÿ�̸ӿ� ����
	float rotY; // ȸ���� ����

	Command_State Attack();
public:
	CommandAttack() {}
	CommandAttack(Actor* actor)
		:actor(actor)
	{}
	~CommandAttack();
	// Command��(��) ���� ��ӵ�
	virtual Command_State Excute() override;

public:
	void SetActor(Actor* actor) { this->actor = actor; }
	void SetAnim(int anim) { animNum = anim; }

public:

	virtual void Update(shared_ptr<BlackBoard> board) override;
	virtual void LateUpdate(shared_ptr<BlackBoard> board) override;
};

