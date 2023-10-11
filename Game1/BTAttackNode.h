#pragma once
class BTAttackNode : public BTNode
{
public:
	BTAttackNode()
	{}

	~BTAttackNode();

public:
	virtual void Set(Actor* actor) override;

	// BTNode을(를) 통해 상속됨
	virtual Command_State Excute() override;
};

