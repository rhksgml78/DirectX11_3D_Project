#include "stdafx.h"

CommandChase::~CommandChase()
{
}

Command_State CommandChase::Excute()
{
	return Chase();
}

void CommandChase::Update(shared_ptr<BlackBoard> board)
{
	if (board)
	{
		targetPos = board->GetObj()->GetWorldPos();
		terrain = board->GetTerrain();
		foundRoute = board->foundRoute;
		range = board->GetRange();

		if (way.empty() or lostTarget)
		{
			way = board->GetWay();
			if (!way.empty())
				lastPos = way.front();
		}

		lostTarget = board->lostTarget;
	}
	else
		return;
}

void CommandChase::LateUpdate(shared_ptr<BlackBoard> board)
{
	if (board)
	{
		board->SetDir(dir);
		if (finish)
			board->SetWay(way);
		board->foundRoute = foundRoute;
		board->lostTarget = lostTarget;
		board->finish = finish;
	}
}

Command_State CommandChase::Chase()
{
	float dis = (targetPos - actor->GetWorldPos()).Length();

	if (dis <= range)
	{
		return Command_State::Success;
	}
	else // Ÿ���� �־����� ���� ����
	{
		return Command_State::Failure;
	}

	//Vector3 currentPos = lastPos - targetPos;
	//float currentLen = currentPos.Length();

	//if (currentLen >= 5.0f) // Ÿ�� �н�
	//{
	//	lostTarget = true;
	//	finish = false;
	//	foundRoute = false;
	//	cout << "[Chase]�Ÿ��� �־����� Ÿ���Ҿ����. Command_State::Failure ��ȯ" << endl;
	//	return Command_State::Failure;
	//}

	////if (!way.empty())
	////{
	////	dir = way.back() - actor->GetWorldPos();
	////	float len = dir.Length();

	////	// ���� ���� �����Ÿ� ����
	////	if (way.size() == 1 and len <= 5.0f)
	////	{
	////		way.pop_back();
	////		dir = Vector3();
	////		foundRoute = false;
	////		lostTarget = false;
	////		finish = true;

	////		cout << "[Chase]�������ִ±��� ũ�Ⱑ 1�̰� �����Ÿ����� �巷����. Command_State::Success ��ȯ" << endl;
	////		return Command_State::Success;
	////	}
	////	else
	////	{
	////		if (len <= 1.0f)
	////		{
	////			way.pop_back();
	////		}
	////	}

	////	cout << "[Chase]�����Ÿ��������⶧����.��Ž��?? Command_State::Running ��ȯ" << endl;
	////	return Command_State::Running;
	////}
	//if(!finish)
	//{
	//	dir = targetPos - actor->GetWorldPos();
	//	float len = dir.Length();

	//	// ���� ���� �����Ÿ� ����
	//	if (len < 5.0f)
	//	{
	//		dir = Vector3();
	//		foundRoute = false;
	//		lostTarget = false;
	//		finish = true;

	//		cout << "[Chase]�������ִ±��� ũ�Ⱑ 1�̰� �����Ÿ����� �巷����. Command_State::Success ��ȯ" << endl;
	//		return Command_State::Success;
	//	}

	//}
	////cout << "[Chase]�����Ÿ��������⶧����. Command_State::Failure ��ȯ" << endl;
	////return Command_State::Failure;
	//cout << "[Chase]�����Ÿ��������⶧����. Command_State::Success ��ȯ" << endl;
	//return Command_State::Success;
}


void CommandChase::CheckState()
{
	if (actor != nullptr)
		isActive = true;
}
