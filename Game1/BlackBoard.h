#pragma once

class BlackBoard
{
private:
	// 추적할 객체
	const GameObject* obj;

	// 찾은 경로
	std::list<Vector3> way;

	// 컴퓨트 셰이딩을 위한 터레인 주소
	Terrain* terrain;

	// 그 외에 AI가 필요로 하는 자료들을 저장할 공간을 마련
	Vector3 dir; // 움직일 방향
	float spd;
	float dis; // 타겟과의 거리

	float timer;
	float range;
	float moverange;
	float introrange;

	// 애니메이션 인덱스 (트레일 적용하기위한 공용 변수)
	int animNum;



public:
	string name;

	// 인트로전용
	bool isintro = true;

	// 데미지
	float damage = 100.0f;

	// 길찾기
	bool foundRoute = false; // 길을 찾았는가?
	bool finish = false; // 도착했는가?
	bool lostTarget = false; // 타겟의 위치를 잃어버렸는가?

	// 공격 판단 함수
	bool isAttack = false;
	bool isTargetHit = false;
	bool isHitting = false;

public:
	BlackBoard();
	~BlackBoard();

public:
	// 자료 입력
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

	// 자료 참조
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

