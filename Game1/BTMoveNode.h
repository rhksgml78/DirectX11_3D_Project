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

	// BTNode��(��) ���� ��ӵ�
	virtual Command_State Excute() override;
};

