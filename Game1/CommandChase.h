#pragma once
class CommandChase : public Command
{
private:
	Actor* actor;

	list<Vector3> way;
	Vector3 targetPos;
	Vector3 dir;
	Vector3 lastPos;
	float dis;
	float range; // �ν� ����

	const Terrain* terrain;

	bool foundRoute;
	bool finish;
	bool lostTarget;

private:
	Command_State Chase();

public:
	CommandChase()
	{};
	CommandChase(Actor* actor)
		:actor(actor)
	{}
	~CommandChase();


	// Command��(��) ���� ��ӵ�
	virtual Command_State Excute() override;
	virtual void Update(shared_ptr<BlackBoard> board) override;
	virtual	void LateUpdate(shared_ptr<BlackBoard> board) override;

	void CheckState();
};

