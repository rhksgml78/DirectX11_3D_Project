#pragma once
class BTIntroNode : public BTNode
{
public:
	BTIntroNode()
	{
	}

	~BTIntroNode();

public:
	// BTNode��(��) ���� ��ӵ�
	virtual void Set(Actor* actor) override;

	virtual Command_State Excute() override;
};