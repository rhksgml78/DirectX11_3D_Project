#pragma once
class BTJumpNode : public BTNode
{
public:
	BTJumpNode()
	{

	}
	~BTJumpNode();

public:
	virtual void Set(Actor* actor) override;

	// BTNode��(��) ���� ��ӵ�
	virtual Command_State Excute() override;
};
