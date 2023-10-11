#pragma once
class BTSequnceNode : public BTNode
{
private:
	vector<BTNode*> nodeList;

public:
	BTSequnceNode()
	{
		this->type = Node_TYPE::SequnceNode;
	}
	~BTSequnceNode();

public:
	// BTNode을(를) 통해 상속됨
	void PushNode(BTNode* node) { nodeList.push_back(node); }


	virtual Command_State Excute() override;

	// BTNode을(를) 통해 상속됨
	virtual void Set(Actor* actor) override;
};

