#include "stdafx.h"

void BehaviorTree::Init(Actor* actor)
{
	root = actor;

	rootNode = new BTSequnceNode();
	board = make_shared<BlackBoard>();

	BTNode* death = new BTDecoratorNode();
	//BTNode* Intro = new BTSequnceNode();
	BTNode* Search = new BTSequnceNode();
	BTNode* Attack = new BTSequnceNode();

	//BTNode* intro = new BTIntroNode();
	BTNode* search = new BTSearchNode();
	BTNode* chase = new BTChaseNode();
	BTNode* move = new BTMoveNode();
	BTNode* attack = new BTAttackNode();

	//if (root->actortype == Actor::ActorType::BOSS)
	{
		// ��� Actor ����
		//intro->Set(actor);
		search->Set(actor);
		chase->Set(actor);
		move->Set(actor);

		attack->Set(actor);

		// ��� ���� ����
		//intro->SetBoard(board);
		search->SetBoard(board);
		chase->SetBoard(board);
		move->SetBoard(board);

		attack->SetBoard(board);

		// Ʈ�� �����ϱ�
		rootNode->PushNode(death);
		//rootNode->PushNode(Intro);
		rootNode->PushNode(Search);
		rootNode->PushNode(Attack);

		//((BTSequnceNode*)Intro)->PushNode(intro);
		((BTSequnceNode*)Search)->PushNode(search);
		((BTSequnceNode*)Search)->PushNode(chase);
		((BTSequnceNode*)Search)->PushNode(move);

		((BTSequnceNode*)Attack)->PushNode(attack);
		//death->SetBoard(board);
		//Search->SetBoard(board);
	}


}

void BehaviorTree::Update()
{
	Vector3 disTemp = root->GetWorldPos() - board->GetObj()->GetWorldPos();
	float dis = disTemp.Length();

	board->SetDistance(dis);

	rootNode->Excute();
}