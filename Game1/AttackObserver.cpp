#include "stdafx.h"

AttackObserver::AttackObserver()
{
}

AttackObserver::~AttackObserver()
{
}

void AttackObserver::Update()
{
	if (!col.empty() and !isHit)
	{
		for (auto it : col)
		{
			if (root->Intersect(it))
			{
				isHit = true;
				col.clear();
				return;
			}
		}
	}
}
