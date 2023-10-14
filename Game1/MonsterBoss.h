#pragma once

class MonsterBoss : public Monster
{
public:
	enum class Boss_State
	{
		NONE,
		INTRO,
		NORMAL,
		HIT,
		PARRY,
		PARRY2,
		CHANGE,
		SKILL,
		ATTACK,
		DEATH
	};

private:
	int		hp = 100;
	int		maxhp = hp;
	int		power = 5;
	// 스킬 관련
	Actor* rootpoint;
	// 0번 원형 타입 스킬
	RocksEffect* rocks1;
	RocksEffect* rocks2;
	// 1번 일자 타입 스킬
	RocksEffectVt* rocks3;
	// 스킬 이펙트 콜라이더 관련
	Actor* Skill_Circle_Col; // 원형 범위 콜라이더
	Actor* Skill_Line_Col; // 직선 범위 콜라이더

	int idx = 0;
	float timer;

	Ray rayToTarget;

	bool isIntersect = false;
	bool isChasing = false;

	Vector3 lastPos;

	// 행동트리 생성
	BehaviorTree* BT;

	// 트레일 생성
	SlashTrail* trail;
	bool		trailEffect;

	// 특수효과
	ExploedEffect* effect;
	DeathEffect* deatheffect;
	RoarEffect* roareffect;
	bool isroar = false;
	Color original;


	float g;

	// 스킬 이펙트 관련
	bool	isskill;
	bool	skillactive = false;
	float	skillzerotime = 0.0f;
	int		skillzerocount = 0;
	bool	isstart = false;
	int		skilltype; // 스킬타입2가지로 나누기 1번:일자,0번:원형
	bool	isskilltonormal = false;

	// 페이즈 전환관련
	bool	ischange = false;
	bool	ischangestart = false;
	float	emissivechange = 0.0f;


	// 스킬 발동을 위한 조건
	int		hitcount = 0;

	// 사운드 실행 관련
	bool	diesound = false;

	// 콜라이더 관련
	Collider* AtkCol1;
	Collider* AtkCol2;
	Collider* HitCol;

private:
	// 옵저버 패턴 관련
	IObserver* colObs;
	IObserver* atkObs;
	ISubject* atkSub;

public:
	Actor* root;

	//보스의 상태 관련 변수
	Boss_State bState;
	
	// 인트로 이벤트용 변수
	bool	isintro;

	// 보스 사망 관련 변수
	int		deathcount;
	float	deathcolor = 0.5f;

	//MoveSubject* moveSub;
	//AttackObserver* atkObs;
	//AttackSubject* atkSub;

	void CheckIntersect(bool tf) { isIntersect = tf; }
	void SetPos(Vector3 pos) { root->SetWorldPos(pos); }

	void Hi() { root->RenderHierarchy(); }

	shared_ptr<BlackBoard> GetBoard() { return BT->board; }

	const Vector3	GetPos()		{ return root->GetWorldPos(); }
	const Vector3	GetLastPos()	{ return lastPos; }
	Actor*			GetObj()		{ return root; }
	Ray				GetRay()		{ return rayToTarget; }
	int				GetSkillType()	{ return skilltype;}

	void			SetState() { this->bState = bState;}

	// 콜라이더
	Collider* GetSkillCol1() { return Skill_Circle_Col->collider; }
	Collider* GetSkillCol2() { return Skill_Line_Col->collider; }
	Collider* GetAktCol1() { return AtkCol1; }
	Collider* GetAktCol2() { return AtkCol2; }
	Collider* GetHitCol() { return HitCol; }
	void	  SetAktCol1(bool tnf) { root->Find("mixamorig:LeftHand")->collider->colOnOff = tnf; }
	void	  SetAktCol2(bool tnf) { root->Find("mixamorig:RightHand")->collider->colOnOff = tnf; }
	void	  SetHitCol(bool tnf) { root->Find("mixamorig:Hips")->collider->colOnOff = tnf; }
	float	  coliderzerotime = 0.0f; // 콜라이더 온 타이머용

	// 그림자용
	RocksEffect*	GetRocks1() { return rocks1; }
	RocksEffect*	GetRocks2() { return rocks2; }
	RocksEffectVt*	GetRocks3() { return rocks3; }

	// 트레일정보
	SlashTrail* GetTrail() { return trail; }

	// Monster를 통해 상속된 가상함수 재정의
	virtual void Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void RenderHierarchy() override;

	void trailAction();
	void RockPosSet0();
	void RockPosSet1();

	Boss_State		GetState()		{ return bState; }

	// 보스의 상태에 따라 실행할 함수 나누기
	void intro();
	void normal();
	void hit();
	void skill();
	void change();
	void parry();
	void parry2();
	void attack();
	void death();

	// 보스 데미지 받기
	void GetDameged(int damage); //플레이어의 공격력 만큼 데미지 받기
	// 콜라이더 다시 켜는 함수
	void ColliderOn();
	float colliderzero = 0.0f;

	int  GetHP() { return hp; }
	int  GetMaxHP() { return maxhp; }

	bool isDameged = false;
	float Damegedzerotime = 0.0f;

	// 보스 패리 상태일때 관련
	float parrytimer = 0.0f;
	int parrycount = 0;
	bool parrystart = false;
	bool parrystart2 = false;

	// 공격력 관련 함수
	int	 GetPower() { return power; }

	// 옵저버 관련
	void SetObs(IObserver* obs) { atkSub->AddObserver(obs); }

	// 스킬 타입 세팅
	void SetSkills(int type) { skilltype = type; isskill = true; } // 0번 직선 공격, 1번 원형 공격

};

