#pragma once
class BTAttackNode : public BTNode
{
public:
	BTAttackNode()
	{}

	~BTAttackNode();

public:
	virtual void Set(Actor* actor) override;

	// BTNode��(��) ���� ��ӵ�
	virtual Command_State Excute() override;
};

