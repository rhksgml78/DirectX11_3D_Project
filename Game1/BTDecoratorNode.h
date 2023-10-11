#pragma once
class BTDecoratorNode : public BTNode
{
private:
	vector<BTNode*> nodeList;

public:

	// BTNode을(를) 통해 상속됨
	virtual Command_State Excute() override;
	virtual void Set(Actor* actor) override;
};