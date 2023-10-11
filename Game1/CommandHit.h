#pragma once
class CommandHit : public Command
{
private:
	Actor* actor;

	Command_State Hit();
public:
	CommandHit() {}
	CommandHit(Actor* actor)
		:actor(actor)
	{}
	~CommandHit();

	// Command을(를) 통해 상속됨
	virtual Command_State Excute() override;
	virtual void Update(shared_ptr<BlackBoard> board) override;
};

