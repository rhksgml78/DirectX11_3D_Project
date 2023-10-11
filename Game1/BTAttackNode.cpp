#include "stdafx.h"

Command_State BTAttackNode::Excute()
{
    command->Update(board);
    Command_State state = command->Excute();
    command->LateUpdate(board);
    return state;
}

BTAttackNode::~BTAttackNode()
{
}

void BTAttackNode::Set(Actor* actor)
{
    command = new class CommandAttack(actor);
}
