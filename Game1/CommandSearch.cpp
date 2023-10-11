#include "stdafx.h"


Command_State CommandSearch::Excute()
{
	return Search();
}

void CommandSearch::Update(shared_ptr<BlackBoard> board)
{
	if (board)
	{
		target = board->GetObj();
		pos = target->GetWorldPos();
		pos.y = 0.0f;
		terrain = board->GetTerrain();
		range = board->GetRange();
		foundRoute = board->foundRoute;
		finish = board->finish;
		if (finish)
			way.clear();
		lostTarget = board->lostTarget;
	}
	else
		return;
}

void CommandSearch::LateUpdate(shared_ptr<BlackBoard> board)
{
	if (board)
	{
		if (board->GetWay().empty())
			board->SetWay(way);
		else if (lostTarget)
		{
			board->SetWay(way);
			lostTarget = false;
		}

		board->foundRoute = foundRoute;
		board->lostTarget = lostTarget;
	}
}


Command_State CommandSearch::Search()
{
	float dis = (pos - actor->GetWorldPos()).Length();

	if (dis < range) // �Ÿ��� ����. ���ذŸ����� ������ Ž�� ����
	{
		return Command_State::Success;
	}
	else // ũ�� ����
	{
		return Command_State::Failure;
	}

	//if (finish == true)
	//{
	//	cout << "[Search]Ž�� �Ϸ�, Command_State::Success ��ȯ" << endl;
	//	return Command_State::Success;
	//}

	//if (foundRoute == true and !lostTarget)
	//{
	//	cout << "[Search]��ã�� Ÿ��ã������ Command_State::Success ��ȯ" << endl;
	//	return Command_State::Success;
	//}


	// ���Ϳ� Ÿ�� ������ �Ÿ��� ��
	// Ÿ���� ��ġ�� ������ ��ġ�� ������ �� �޶����� ���, foundRoute�� �����ؼ� �ٽ� �˻��ϴ� �������� ������ ����


	//if (dis < range)
	//{
	//	Ray actToTarget;
	//	actToTarget.position = actor->GetWorldPos();
	//	actToTarget.direction = pos - Vector3(actor->GetWorldPos().x, 0.0f, actor->GetWorldPos().z);

	//	if (actToTarget.direction == Vector3(0, 0, 0)) // ���ͷκ��� Ÿ���� ��ġ������ ���⺤�Ͱ� 0�ϋ�. �� ������ġ!
	//	{
	//		cout << "[Search]���Ϳ� ������ġ��, Command_State::Failure ��ȯ" << endl;
	//		return Command_State::Failure;
	//	}
	//	else
	//	{
	//		foundRoute = true;
	//		return Command_State::Failure;

	//	}
	//	//// ���� ��ġ�� �ƴҰ�쿡 �Ʒ��� �����
	//	//way.clear(); // Ž����θ� ����ְ�

	//	////if (way.empty()) // ���� ����ִٸ�
	//	//if (terrain->IntersectCol(actToTarget)) // Ÿ�ٹ��������� ���̰� �ݶ��̴��� �浹�ߴٸ�
	//	//{
	//	//	terrain->di.AddNode(pos); // ���ͽ�Ʈ���� ��忡 Ÿ���� ��ġ�� �߰���Ű��
	//	//	terrain->AllLink(pos); // ���ͽ�Ʈ�� ��ã�� ����Ʈ�� ������ Ÿ���� ��ġ�� �����Ų��!

	//	//	src = terrain->FindNodeIdx(actToTarget.position); // ������ ��ġ ����
	//	//	dest = terrain->FindNodeIdx(pos); // Ÿ���� ��ġ ����
	//	//	terrain->di.list[dest]->diType = DiNode_TYPE::PATHNODE;

	//	//	if (terrain->PathFinding(src, dest, way)) //��ã�� ����
	//	//	{
	//	//		cout << "[Search]��ã�����ǹ� ������ foundRoute = true ��ȯ" << endl;

	//	//		int dest = terrain->FindNodeIdx(way.front()); // ��ã���� ���������� ����
	//	//		if (terrain->di.list[dest]->diType == DiNode_TYPE::PATHNODE) // Ÿ���� ��ġ�ϸ�
	//	//			terrain->di.PopNode(dest); // ���� ����� 

	//	//		foundRoute = true;
	//	//	}
	//	//	else
	//	//	{
	//	//		cout << "[Search]��ã�����ǹ�X Command_State::Failure ��ȯ" << endl;

	//	//		return Command_State::Failure;
	//	//		//return Command_State::Success;
	//	//	}
	//	//}
	//	//else
	//	//{
	//	//	cout << "[Search]way�� ������ġ Ǫ�ù���!" << endl;
	//	//	way.push_back(pos);

	//	//	foundRoute = true;
	//	//}
	//}

	//cout << "[Search]���ǹ����ɸ����ʰ� Command_State::Success ��ȯ" << endl;
	//return Command_State::Success;
}