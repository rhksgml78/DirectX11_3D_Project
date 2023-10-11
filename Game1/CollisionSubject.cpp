#include "stdafx.h"

void CollisionSubject::Notify()
{
	for (auto it : obsList)
	{
		dynamic_cast<CollisionObserver*>(it.second)->SetList(colList);
		dynamic_cast<CollisionObserver*>(it.second)->Update();
	}
}
