#pragma once
class BTSearchNode : public BTNode
{
public:
	BTSearchNode()
	{}

	~BTSearchNode();

public:
	virtual void Set(Actor* actor) override;

	// BTNode을(를) 통해 상속됨
	virtual Command_State Excute() override;
};

