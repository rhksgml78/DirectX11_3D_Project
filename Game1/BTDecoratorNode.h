#pragma once
class BTDecoratorNode : public BTNode
{
private:
	vector<BTNode*> nodeList;

public:

	// BTNode��(��) ���� ��ӵ�
	virtual Command_State Excute() override;
	virtual void Set(Actor* actor) override;
};