#pragma once

class CommandIntro : public Command
{
private:
	Actor* actor;
	Vector3 pos;

	float introrange; // 인식 범위

	Terrain* terrain;
	const GameObject* target;

	bool bossintro;
	int ischange = 0;

	Command_State Intro();

public:

	CommandIntro()
	{
	}
	CommandIntro(Actor* actor)
		:actor(actor)
	{
	}
	~CommandIntro() {}

public:
	// Command을(를) 통해 상속됨
	virtual Command_State Excute() override;
	virtual void Update(shared_ptr<BlackBoard> board) override;
	virtual	void LateUpdate(shared_ptr<BlackBoard> board) override;
};

