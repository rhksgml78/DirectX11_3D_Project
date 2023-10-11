#include "stdafx.h"

BTSequnceNode::~BTSequnceNode()
{
}

Command_State BTSequnceNode::Excute()
{
	Command_State state = Command_State::Failure;

	for (auto it = nodeList.begin(); it != nodeList.end(); ++it)
	{
		state = (*it)->Excute();
		if (state == Command_State::Failure or state == Command_State::Running)
		{
			return state;
		}
	}

	return state;
}

void BTSequnceNode::Set(Actor* actor)
{

}
