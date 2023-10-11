#include "stdafx.h"

BTSelectorNode::~BTSelectorNode()
{
}

Command_State BTSelectorNode::Excute()
{
	Command_State state = Command_State::Failure;

	for (auto it = nodeList.begin(); it != nodeList.end(); ++it)
	{
		state = (*it)->Excute();
		if (state == Command_State::Success or state == Command_State::Running)
		{
			return state;
		}
	}

	return state;
}

void BTSelectorNode::Set(Actor* actor)
{
}