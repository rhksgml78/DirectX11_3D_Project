#pragma once
class BTNode
{
protected:
	enum class Node_TYPE
	{
		LeafNode,
		SelectorNode,
		SequnceNode,
		DecoratorNode
	};
protected:
	// 각 노드는 하나의 명령(행동)을 수행
	Command* command;

	// 추후 트리 저장을 위한 열거형
	Node_TYPE type;

	// 참조할 블랙보드 -> 추적할 객체, 현재 타겟과의 거리 등, 필요한 정보들을 갖춘 자료 저장 객체, 향후 GetSet으로 커맨드 하위 객체들을 통해 호출 및 값 설정
	shared_ptr<BlackBoard> board;

public:
	BTNode();
	virtual ~BTNode();

public:
	virtual Command_State Excute() = 0;
	virtual void Set(Actor* actor) = 0;

	void SetBoard(shared_ptr<BlackBoard> board) { this->board = board; }

	const shared_ptr<BlackBoard> GetBoard() { return this->board; }
};

