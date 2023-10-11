#include "stdafx.h"

Command_State CommandIntro::Excute()
{
	return Intro();
}

Command_State CommandIntro::Intro()
{
	// 인트로 실행
	
	return Command_State::Running;

}

void CommandIntro::Update(shared_ptr<BlackBoard> board)
{
	if (board)
	{
		target = board->GetObj();
		pos = target->GetWorldPos();
		pos.y = 0.0f;
		terrain = board->GetTerrain();
	}
	else
		return;
}

void CommandIntro::LateUpdate(shared_ptr<BlackBoard> board)
{
}
