#pragma once
class BTSearchNode : public BTNode
{
public:
	BTSearchNode()
	{}

	~BTSearchNode();

public:
	virtual void Set(Actor* actor) override;

	// BTNode��(��) ���� ��ӵ�
	virtual Command_State Excute() override;
};

