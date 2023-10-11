#include "framework.h"

Grid* Grid::Create(string name)
{
	Grid* temp = new Grid();
	temp->LoadFile("Grid.xml");

	return temp;
}
void Grid::Release()
{
	for (auto it = children.begin(); it != children.end(); it++)
	{
		SafeRelease(it->second);
	}
	delete this;
}

Grid::Grid()
{
}

Grid::~Grid()
{
}

void Grid::Update()
{
	scale.z = scale.x;

	float X = (int)(Camera::main->GetWorldPos().x / scale.x);
	X *= scale.x;

	float Z = (int)(Camera::main->GetWorldPos().z / scale.x);
	Z *= scale.x;

	SetWorldPosX(X);
	SetWorldPosZ(Z);

	Actor::Update();
}

