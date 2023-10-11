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
	// �� ���� �ϳ��� ���(�ൿ)�� ����
	Command* command;

	// ���� Ʈ�� ������ ���� ������
	Node_TYPE type;

	// ������ ������ -> ������ ��ü, ���� Ÿ�ٰ��� �Ÿ� ��, �ʿ��� �������� ���� �ڷ� ���� ��ü, ���� GetSet���� Ŀ�ǵ� ���� ��ü���� ���� ȣ�� �� �� ����
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

