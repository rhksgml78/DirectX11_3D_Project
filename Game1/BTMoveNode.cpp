#include "stdafx.h"

Command_State BTMoveNode::Excute()
{
    command->Update(board);
    Command_State state = command->Excute();
    command->LateUpdate(board);
    return state;
}

BTMoveNode::~BTMoveNode()
{
}

void BTMoveNode::Set(Actor* actor)
{
    command = new class CommandMove(actor);
}
