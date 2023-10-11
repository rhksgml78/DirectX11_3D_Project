#pragma once
class CommandSearch : public Command
{
private:
	Actor* actor;
	Vector3 pos;

	float range; // 인식 범위

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
	// Command을(를) 통해 상속됨
	virtual Command_State Excute() override;
	virtual void Update(shared_ptr<BlackBoard> board) override;
	virtual	void LateUpdate(shared_ptr<BlackBoard> board) override;
};

