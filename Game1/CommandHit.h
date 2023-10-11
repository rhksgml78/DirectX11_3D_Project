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

	// Command��(��) ���� ��ӵ�
	virtual Command_State Excute() override;
	virtual void Update(shared_ptr<BlackBoard> board) override;
};

