#include "stdafx.h"

CommandHit::~CommandHit()
{
}

Command_State CommandHit::Excute()
{
	return Hit();
}

void CommandHit::Update(shared_ptr<BlackBoard> board)
{
	if (board)
	{
	}
	else
		return;
}

Command_State CommandHit::Hit()
{
	cout << "Hit ����" << endl;

	return Command_State::Running;
}