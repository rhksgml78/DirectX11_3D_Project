#include "stdafx.h"

Command_State BTChaseNode::Excute()
{
    command->Update(board);
    Command_State state = command->Excute();
    command->LateUpdate(board);
    return state;
}

BTChaseNode::~BTChaseNode()
{
}

void BTChaseNode::Set(Actor* actor)
{
    command = new class CommandChase(actor);
}
