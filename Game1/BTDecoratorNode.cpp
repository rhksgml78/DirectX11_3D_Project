#include "stdafx.h"

Command_State BTDecoratorNode::Excute()
{
	Command_State state = Command_State::Success;

	for (auto it : nodeList)
	{
		state = it->Excute();
		if (state == Command_State::Success)
		{
			state = Command_State::Failure;
		}
		else
			state = Command_State::Success;
	}

	return state;
}

void BTDecoratorNode::Set(Actor* actor)
{

}