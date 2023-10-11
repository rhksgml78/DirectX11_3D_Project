#include "stdafx.h"

BTIntroNode::~BTIntroNode()
{
}

void BTIntroNode::Set(Actor* actor)
{
    command = new class CommandIntro(actor);
}

Command_State BTIntroNode::Excute()
{
    command->Update(board);
    Command_State state = command->Excute();
    command->LateUpdate(board);
    return state;
}
