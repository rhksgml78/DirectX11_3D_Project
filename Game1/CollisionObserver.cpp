#include "stdafx.h"

void CollisionObserver::Update()
{
	if (!colList.empty() and col)
	{
		for (auto it : colList)
		{
			if (it->Intersect(col))
			{
				isCollision = true;
			}
			else
				isCollision = false;
		}
	}
}
