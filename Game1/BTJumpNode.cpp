#include "stdafx.h"

Command_State BTJumpNode::Excute()
{
    command->Update(board);
    Command_State state = command->Excute();
    command->LateUpdate(board);
    return state;
}


BTJumpNode::~BTJumpNode()
{
}

void BTJumpNode::Set(Actor* actor)
{
    command = new class CommandJump(actor);
}
