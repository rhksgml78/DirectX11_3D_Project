#include "stdafx.h"
#include "Solar.h"

Solar* Solar::Create(string name)
{
	Solar* sun = new Solar();
	sun->LoadFile("Sun.xml");

	

	return sun;
}
void Solar::Release()
{
	for (auto it = children.begin(); it != children.end(); it++)
	{
		SafeRelease(it->second);
	}
	delete this;
}
Solar::Solar()
{

}

Solar::~Solar()
{
}

void Solar::Update()
{

	/*if (INPUT->KeyPress(VK_UP))
	{
		MoveWorldPos(GetForward() * 10 * DELTA);
	}




	if (GameObject* p = Find("MercuryBorn"))
	{
		p->rotation.y += DELTA * 3.14;
	}
	Find("VenusBorn")->rotation.y += DELTA * 3.14;*/

	Actor::Update();





}


