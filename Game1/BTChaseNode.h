#pragma once
class BTChaseNode : public BTNode
{
public:
	BTChaseNode()
	{
	}

	~BTChaseNode();

public:
	// BTNode��(��) ���� ��ӵ�
	virtual void Set(Actor* actor) override;

	virtual Command_State Excute() override;
};

