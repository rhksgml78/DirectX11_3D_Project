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
	// BTNode��(��) ���� ��ӵ�
	void PushNode(BTNode* node) { nodeList.push_back(node); }


	virtual Command_State Excute() override;

	// BTNode��(��) ���� ��ӵ�
	virtual void Set(Actor* actor) override;
};

