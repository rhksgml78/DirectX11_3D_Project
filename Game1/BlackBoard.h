#pragma once

class BlackBoard
{
private:
	// ������ ��ü
	const GameObject* obj;

	// ã�� ���
	std::list<Vector3> way;

	// ��ǻƮ ���̵��� ���� �ͷ��� �ּ�
	Terrain* terrain;

	// �� �ܿ� AI�� �ʿ�� �ϴ� �ڷ���� ������ ������ ����
	Vector3 dir; // ������ ����
	float spd;
	float dis; // Ÿ�ٰ��� �Ÿ�

	float timer;
	float range;
	float moverange;
	float introrange;

	// �ִϸ��̼� �ε��� (Ʈ���� �����ϱ����� ���� ����)
	int animNum;



public:
	string name;

	// ��Ʈ������
	bool isintro = true;

	// ������
	float damage = 100.0f;

	// ��ã��
	bool foundRoute = false; // ���� ã�Ҵ°�?
	bool finish = false; // �����ߴ°�?
	bool lostTarget = false; // Ÿ���� ��ġ�� �Ҿ���ȴ°�?

	// ���� �Ǵ� �Լ�
	bool isAttack = false;
	bool isTargetHit = false;
	bool isHitting = false;

public:
	BlackBoard();
	~BlackBoard();

public:
	// �ڷ� �Է�
	void SetTerrain(Terrain* terrain)		{ this->terrain = terrain; }
	void SetObj(const GameObject* obj)		{ this->obj = obj;}
	void SetWay(std::list<Vector3>& way)	{ this->way = way; }
	void SetSpd(float spd)					{ this->spd = spd; }
	void SetDir(Vector3 dir)				{ this->dir = dir; }
	void SetDistance(float dis)				{ this->dis = dis; }
	void SetRange(float range)				{ this->range = range; }
	void SetMoveRange(float moverange)		{ this->moverange = moverange; }
	void SetAnimNum(int animNum)			{ this->animNum = animNum; }
	void SetIntroRange(int introrange)		{ this->introrange = introrange; }
	void Setintro(bool isintro)				{ this->isintro = isintro; }

	// �ڷ� ����
	std::list<Vector3>		GetWay() { return way; }
	Vector3					GetDir() { return dir; }
	const GameObject*		GetObj() const { return obj; }
	Terrain*				GetTerrain() const { return terrain; }
	float					GetSpeed() { return spd; }
	float					GetDistance() { return dis; }
	float*					GetTimer() { return &timer; };
	float					GetRange() { return range; }
	float					GetMoveRange() { return moverange; }
	float					GetIntroRange() { return introrange; }
	int						GetAnimnum() { return animNum; }
	bool					Getintro() { return isintro; }
	bool					GetisAttack() { return isAttack; }

};

