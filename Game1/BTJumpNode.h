#pragma once
class BTJumpNode : public BTNode
{
public:
	BTJumpNode()
	{

	}
	~BTJumpNode();

public:
	virtual void Set(Actor* actor) override;

	// BTNode을(를) 통해 상속됨
	virtual Command_State Excute() override;
};
