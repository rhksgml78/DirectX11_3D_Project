#pragma once
class CommandSearch : public Command
{
private:
	Actor* actor;
	Vector3 pos;

	float range; // �ν� ����

	Terrain* terrain;
	const GameObject* target;
	std::list<Vector3> way;

	bool foundRoute = false;
	bool finish = false;
	bool lostTarget = false;

	int src = 0, dest = 0;

	Command_State Search();

public:
	CommandSearch()
	{
	}
	CommandSearch(Actor* actor)
		:actor(actor)
	{
	}
	~CommandSearch() {}

public:
	// Command��(��) ���� ��ӵ�
	virtual Command_State Excute() override;
	virtual void Update(shared_ptr<BlackBoard> board) override;
	virtual	void LateUpdate(shared_ptr<BlackBoard> board) override;
};

