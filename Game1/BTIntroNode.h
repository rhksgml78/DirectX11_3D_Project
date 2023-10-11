#pragma once
class BTIntroNode : public BTNode
{
public:
	BTIntroNode()
	{
	}

	~BTIntroNode();

public:
	// BTNode을(를) 통해 상속됨
	virtual void Set(Actor* actor) override;

	virtual Command_State Excute() override;
};