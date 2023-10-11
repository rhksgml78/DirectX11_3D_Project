#pragma once
class BehaviorTree
{
private:
	BTSequnceNode*	rootNode;
	Actor*			root;

public:
	// �ൿ Ʈ�� ���� ��ɵ��� ������ ���� �ڷ�
	shared_ptr<BlackBoard>	board;

	void Init(Actor* actor);
	void Update();
};

