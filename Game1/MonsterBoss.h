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
	// ��ų ����
	Actor* rootpoint;
	// 0�� ���� Ÿ�� ��ų
	RocksEffect* rocks1;
	RocksEffect* rocks2;
	// 1�� ���� Ÿ�� ��ų
	RocksEffectVt* rocks3;
	// ��ų ����Ʈ �ݶ��̴� ����
	Actor* Skill_Circle_Col; // ���� ���� �ݶ��̴�
	Actor* Skill_Line_Col; // ���� ���� �ݶ��̴�

	int idx = 0;
	float timer;

	Ray rayToTarget;

	bool isIntersect = false;
	bool isChasing = false;

	Vector3 lastPos;

	// �ൿƮ�� ����
	BehaviorTree* BT;

	// Ʈ���� ����
	SlashTrail* trail;
	bool		trailEffect;

	// Ư��ȿ��
	ExploedEffect* effect;
	DeathEffect* deatheffect;
	RoarEffect* roareffect;
	bool isroar = false;
	Color original;


	float g;

	// ��ų ����Ʈ ����
	bool	isskill;
	bool	skillactive = false;
	float	skillzerotime = 0.0f;
	int		skillzerocount = 0;
	bool	isstart = false;
	int		skilltype; // ��ųŸ��2������ ������ 1��:����,0��:����
	bool	isskilltonormal = false;

	// ������ ��ȯ����
	bool	ischange = false;
	bool	ischangestart = false;
	float	emissivechange = 0.0f;


	// ��ų �ߵ��� ���� ����
	int		hitcount = 0;

	// ���� ���� ����
	bool	diesound = false;

	// �ݶ��̴� ����
	Collider* AtkCol1;
	Collider* AtkCol2;
	Collider* HitCol;

private:
	// ������ ���� ����
	IObserver* colObs;
	IObserver* atkObs;
	ISubject* atkSub;

public:
	Actor* root;

	//������ ���� ���� ����
	Boss_State bState;
	
	// ��Ʈ�� �̺�Ʈ�� ����
	bool	isintro;

	// ���� ��� ���� ����
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

	// �ݶ��̴�
	Collider* GetSkillCol1() { return Skill_Circle_Col->collider; }
	Collider* GetSkillCol2() { return Skill_Line_Col->collider; }
	Collider* GetAktCol1() { return AtkCol1; }
	Collider* GetAktCol2() { return AtkCol2; }
	Collider* GetHitCol() { return HitCol; }
	void	  SetAktCol1(bool tnf) { root->Find("mixamorig:LeftHand")->collider->colOnOff = tnf; }
	void	  SetAktCol2(bool tnf) { root->Find("mixamorig:RightHand")->collider->colOnOff = tnf; }
	void	  SetHitCol(bool tnf) { root->Find("mixamorig:Hips")->collider->colOnOff = tnf; }
	float	  coliderzerotime = 0.0f; // �ݶ��̴� �� Ÿ�̸ӿ�

	// �׸��ڿ�
	RocksEffect*	GetRocks1() { return rocks1; }
	RocksEffect*	GetRocks2() { return rocks2; }
	RocksEffectVt*	GetRocks3() { return rocks3; }

	// Ʈ��������
	SlashTrail* GetTrail() { return trail; }

	// Monster�� ���� ��ӵ� �����Լ� ������
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

	// ������ ���¿� ���� ������ �Լ� ������
	void intro();
	void normal();
	void hit();
	void skill();
	void change();
	void parry();
	void parry2();
	void attack();
	void death();

	// ���� ������ �ޱ�
	void GetDameged(int damage); //�÷��̾��� ���ݷ� ��ŭ ������ �ޱ�
	// �ݶ��̴� �ٽ� �Ѵ� �Լ�
	void ColliderOn();
	float colliderzero = 0.0f;

	int  GetHP() { return hp; }
	int  GetMaxHP() { return maxhp; }

	bool isDameged = false;
	float Damegedzerotime = 0.0f;

	// ���� �и� �����϶� ����
	float parrytimer = 0.0f;
	int parrycount = 0;
	bool parrystart = false;
	bool parrystart2 = false;

	// ���ݷ� ���� �Լ�
	int	 GetPower() { return power; }

	// ������ ����
	void SetObs(IObserver* obs) { atkSub->AddObserver(obs); }

	// ��ų Ÿ�� ����
	void SetSkills(int type) { skilltype = type; isskill = true; } // 0�� ���� ����, 1�� ���� ����

};

