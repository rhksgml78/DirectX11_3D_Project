#pragma once
class BTMoveNode : public BTNode
{
public:
	BTMoveNode()
	{

	}

	~BTMoveNode();

public:
	virtual void Set(Actor* actor) override;

	// BTNode을(를) 통해 상속됨
	virtual Command_State Excute() override;
};

