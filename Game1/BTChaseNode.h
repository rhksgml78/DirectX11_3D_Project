#pragma once
class BTChaseNode : public BTNode
{
public:
	BTChaseNode()
	{
	}

	~BTChaseNode();

public:
	// BTNode을(를) 통해 상속됨
	virtual void Set(Actor* actor) override;

	virtual Command_State Excute() override;
};

