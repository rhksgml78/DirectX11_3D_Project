#include "stdafx.h"

Command_State BTSearchNode::Excute()
{
    command->Update(board);
    Command_State state = command->Excute();
    command->LateUpdate(board);
    return state;
}

BTSearchNode::~BTSearchNode()
{
}

void BTSearchNode::Set(Actor* actor)
{
    command = new class CommandSearch(actor);
}
