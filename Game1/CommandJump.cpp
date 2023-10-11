#include "stdafx.h"

Command_State CommandJump::Jump()
{
	cout << "Jump ½ÇÇà" << endl;
	//Ray toDownRay;
	//toDownRay.position = actor->GetWorldPos() + Vector3(0, 10, 0);
	//toDownRay.direction = -UP;

	//Vector3 Hit;

	//if (!isJump)
	//{
	//	isJump = true;
	//	gravity = -12.5f;
	//}
	//else
	//{
	//	actor->MoveWorldPos(-UP * gravity * DELTA);
	//	gravity += 10.0f * DELTA;
	//}

	//if (gravity > 0)
	//{
	//	if (computePicking == nullptr)
	//	{
	//		computePicking = &Terrain::ComPutePicking;
	//	}

	//	if (computePicking(*terrain, toDownRay, Hit))
	//	{
	//		float dis = (Hit - actor->GetWorldPos()).Length();

	//		if (dis <= 0.5)
	//		{
	//			isJump = false;
	//			actor->SetWorldPosY(Hit.y);

	//			computePicking = nullptr;

	//			return Command_State::Success;
	//		}
	//	}
	//}

	return Command_State::Running;
}

Command_State CommandJump::Excute()
{
	return Jump();
}

void CommandJump::Update(shared_ptr<BlackBoard> board)
{
	if (board)
	{
		//terrain = board->GetTerrain();
	}
	else
		return;
}

void CommandJump::LateUpdate(shared_ptr<BlackBoard> board)
{
}

void CommandJump::CheckState()
{
	if (actor)
		isActive = true;
}
