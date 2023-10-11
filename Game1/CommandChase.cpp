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
	else // 타겟이 멀어져서 추적 실패
	{
		return Command_State::Failure;
	}

	//Vector3 currentPos = lastPos - targetPos;
	//float currentLen = currentPos.Length();

	//if (currentLen >= 5.0f) // 타겟 분실
	//{
	//	lostTarget = true;
	//	finish = false;
	//	foundRoute = false;
	//	cout << "[Chase]거리가 멀어져서 타겟잃어버림. Command_State::Failure 반환" << endl;
	//	return Command_State::Failure;
	//}

	////if (!way.empty())
	////{
	////	dir = way.back() - actor->GetWorldPos();
	////	float len = dir.Length();

	////	// 적에 대한 사정거리 진입
	////	if (way.size() == 1 and len <= 5.0f)
	////	{
	////		way.pop_back();
	////		dir = Vector3();
	////		foundRoute = false;
	////		lostTarget = false;
	////		finish = true;

	////		cout << "[Chase]가지고있는길의 크기가 1이고 사정거리내로 드렁왔음. Command_State::Success 반환" << endl;
	////		return Command_State::Success;
	////	}
	////	else
	////	{
	////		if (len <= 1.0f)
	////		{
	////			way.pop_back();
	////		}
	////	}

	////	cout << "[Chase]사정거리내에없기때문에.재탐색?? Command_State::Running 반환" << endl;
	////	return Command_State::Running;
	////}
	//if(!finish)
	//{
	//	dir = targetPos - actor->GetWorldPos();
	//	float len = dir.Length();

	//	// 적에 대한 사정거리 진입
	//	if (len < 5.0f)
	//	{
	//		dir = Vector3();
	//		foundRoute = false;
	//		lostTarget = false;
	//		finish = true;

	//		cout << "[Chase]가지고있는길의 크기가 1이고 사정거리내로 드렁왔음. Command_State::Success 반환" << endl;
	//		return Command_State::Success;
	//	}

	//}
	////cout << "[Chase]사정거리내에없기때문에. Command_State::Failure 반환" << endl;
	////return Command_State::Failure;
	//cout << "[Chase]사정거리내에없기때문에. Command_State::Success 반환" << endl;
	//return Command_State::Success;
}


void CommandChase::CheckState()
{
	if (actor != nullptr)
		isActive = true;
}
