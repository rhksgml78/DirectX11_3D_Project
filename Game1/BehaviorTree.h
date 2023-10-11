#pragma once
class BehaviorTree
{
private:
	BTSequnceNode*	rootNode;
	Actor*			root;

public:
	// 행동 트리 내부 명령들이 참조할 정보 자료
	shared_ptr<BlackBoard>	board;

	void Init(Actor* actor);
	void Update();
};

