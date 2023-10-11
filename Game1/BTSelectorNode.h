#pragma once
class BTSelectorNode : public BTNode
{
private:
	vector<BTNode*> nodeList;

public:
	BTSelectorNode()
	{
		this->type = Node_TYPE::SelectorNode;
	}
	~BTSelectorNode();

public:
	// BTNode��(��) ���� ��ӵ�
	void PushNode(BTNode* node) { nodeList.push_back(node); }

	virtual Command_State Excute() override;

	// BTNode��(��) ���� ��ӵ�
	virtual void Set(Actor* actor) override;
};

