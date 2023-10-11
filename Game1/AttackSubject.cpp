#include "stdafx.h"

AttackSubject::AttackSubject()
{
}

AttackSubject::~AttackSubject()
{
}

void AttackSubject::Notify()
{
	for (auto it : obsList)
	{
		dynamic_cast<AttackObserver*>(it.second)->SetCol(col);
		dynamic_cast<AttackObserver*>(it.second)->SetAtk(atk);
		dynamic_cast<AttackObserver*>(it.second)->Update();
	}
}