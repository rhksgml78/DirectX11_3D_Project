#pragma once
class MonsterType1 : public Monster
{
public:
	enum class Monster_State
	{
		STANDBY,
		NORMAL,
		MOVE,
		HIT,
		ATTACK,
		DEATH
	};

private:
	int idx = 0;
	float timer;
	int deathcount = 0;

	Ray rayToTarget;

	bool isIntersect = false;
	bool isChasing = false;
	bool isstandby = true;

	bool ishit = false;

	Vector3 lastPos;

	// 행동트리 생성
	BehaviorTree* BT;

	// 트레일 생성
	SlashTrail* trail;
	bool		trailEffect;

	// 이펙트
	DustEffect* dusteffect;
	bool isdust;
	HitEffect* hiteffect;
	bool is_hit;
	BloodEffect* bloodeffect;
	DeathEffect* deatheffect;

	// 콜라이더 관련
	Collider* AtkCol1;
	Collider* AtkCol2;
	Collider* AtkCol3;
	Collider* HitCol;

public:
	int		hp = 15;
	int		power = 5;

	Actor* root;

	//상태 관련 변수
	Monster_State mState;
	
	//사망시 색상 변경 관련
	float	deathcolor = 0.5f;

	//MoveSubject* moveSub;
	//AttackObserver* atkObs;
	//AttackSubject* atkSub;

	void			CheckIntersect(bool tf) { isIntersect = tf; }
	void			SetPos(Vector3 pos) { root->SetWorldPos(pos); }

	void			Hi() { root->RenderHierarchy(); }

	shared_ptr<BlackBoard> GetBoard() { return BT->board; }

	const Vector3	GetPos() { return root->GetWorldPos(); }
	const Vector3	GetLastPos() { return lastPos; }
	Actor*			GetObj() { return root; }
	Ray				GetRay() { return rayToTarget; }

	void			SetState() { this->mState = mState; }

	// 콜라이더
	Collider* GetAktCol1() { return AtkCol1; }
	Collider* GetAktCol2() { return AtkCol2; }
	Collider* GetAktCol3() { return AtkCol3; }
	Collider* GetHitCol() { return HitCol; }
	void	  SetAktCol1(bool tnf) { root->Find("mixamorig:LeftHand")->collider->colOnOff = tnf; }
	void	  SetAktCol2(bool tnf) { root->Find("mixamorig:RightHand")->collider->colOnOff = tnf; }
	void	  SetAktCol3(bool tnf) { root->Find("mixamorig:RightFoot")->collider->colOnOff = tnf; }
	void	  SetHitCol(bool tnf) { root->Find("mixamorig:Hips")->collider->colOnOff = tnf; }
	
	// 콜라이더 On세팅
	void	  ColOn();
	float	  coliderzerotime = 0.0f; // 콜라이더 온 타이머용

	// Monster를 통해 상속된 가상함수 재정의
	virtual void Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void RenderHierarchy() override;

	void trailAction();
	Monster_State		GetState() { return mState; }

	void		standby();
	void		normal();
	void		hit();
	void		death();

	int  GetHP() { return hp; };

	// 피격 관련 함수
	void GetDameged(int damage);

	// 공격력 관련 함수
	int	 GetPower() { return power; }
	void SetPower();
};

