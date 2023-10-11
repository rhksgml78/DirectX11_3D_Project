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

	if (dis < range) // 거리로 측정. 기준거리보다 작으면 탐색 성공
	{
		return Command_State::Success;
	}
	else // 크면 실패
	{
		return Command_State::Failure;
	}

	//if (finish == true)
	//{
	//	cout << "[Search]탐색 완료, Command_State::Success 반환" << endl;
	//	return Command_State::Success;
	//}

	//if (foundRoute == true and !lostTarget)
	//{
	//	cout << "[Search]길찾고 타겟찾은상태 Command_State::Success 반환" << endl;
	//	return Command_State::Success;
	//}


	// 액터와 타겟 사이의 거리로 비교
	// 타겟의 위치가 종전의 위치와 비교했을 때 달라졌을 경우, foundRoute를 수정해서 다시 검색하는 형식으로 진행할 예정


	//if (dis < range)
	//{
	//	Ray actToTarget;
	//	actToTarget.position = actor->GetWorldPos();
	//	actToTarget.direction = pos - Vector3(actor->GetWorldPos().x, 0.0f, actor->GetWorldPos().z);

	//	if (actToTarget.direction == Vector3(0, 0, 0)) // 몬스터로부터 타겟의 위치까지의 방향벡터가 0일떄. 즉 같은위치!
	//	{
	//		cout << "[Search]몬스터와 같은위치다, Command_State::Failure 반환" << endl;
	//		return Command_State::Failure;
	//	}
	//	else
	//	{
	//		foundRoute = true;
	//		return Command_State::Failure;

	//	}
	//	//// 같은 위치가 아닐경우에 아래가 실행됨
	//	//way.clear(); // 탐색경로를 비워주고

	//	////if (way.empty()) // 길이 비어있다면
	//	//if (terrain->IntersectCol(actToTarget)) // 타겟방향으로의 레이가 콜라이더에 충돌했다면
	//	//{
	//	//	terrain->di.AddNode(pos); // 다익스트라의 노드에 타겟의 위치를 추가시키고
	//	//	terrain->AllLink(pos); // 다익스트라 길찾기 리스트의 모든노드와 타겟의 위치를 연결시킨다!

	//	//	src = terrain->FindNodeIdx(actToTarget.position); // 몬스터의 위치 저장
	//	//	dest = terrain->FindNodeIdx(pos); // 타겟의 위치 저장
	//	//	terrain->di.list[dest]->diType = DiNode_TYPE::PATHNODE;

	//	//	if (terrain->PathFinding(src, dest, way)) //길찾기 실행
	//	//	{
	//	//		cout << "[Search]길찾기조건문 들어왔음 foundRoute = true 반환" << endl;

	//	//		int dest = terrain->FindNodeIdx(way.front()); // 길찾기의 시작지점을 저장
	//	//		if (terrain->di.list[dest]->diType == DiNode_TYPE::PATHNODE) // 타입이 일치하면
	//	//			terrain->di.PopNode(dest); // 앞의 노드팝 

	//	//		foundRoute = true;
	//	//	}
	//	//	else
	//	//	{
	//	//		cout << "[Search]길찾기조건문X Command_State::Failure 반환" << endl;

	//	//		return Command_State::Failure;
	//	//		//return Command_State::Success;
	//	//	}
	//	//}
	//	//else
	//	//{
	//	//	cout << "[Search]way에 현재위치 푸시백중!" << endl;
	//	//	way.push_back(pos);

	//	//	foundRoute = true;
	//	//}
	//}

	//cout << "[Search]조건문에걸리지않고 Command_State::Success 반환" << endl;
	//return Command_State::Success;
}