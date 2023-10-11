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

	// �ൿƮ�� ����
	BehaviorTree* BT;

	// Ʈ���� ����
	SlashTrail* trail;
	bool		trailEffect;

	// ����Ʈ
	DustEffect* dusteffect;
	bool isdust;
	HitEffect* hiteffect;
	bool is_hit;
	BloodEffect* bloodeffect;
	DeathEffect* deatheffect;

	// �ݶ��̴� ����
	Collider* AtkCol1;
	Collider* AtkCol2;
	Collider* AtkCol3;
	Collider* HitCol;

public:
	int		hp = 15;
	int		power = 5;

	Actor* root;

	//���� ���� ����
	Monster_State mState;
	
	//����� ���� ���� ����
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

	// �ݶ��̴�
	Collider* GetAktCol1() { return AtkCol1; }
	Collider* GetAktCol2() { return AtkCol2; }
	Collider* GetAktCol3() { return AtkCol3; }
	Collider* GetHitCol() { return HitCol; }
	void	  SetAktCol1(bool tnf) { root->Find("mixamorig:LeftHand")->collider->colOnOff = tnf; }
	void	  SetAktCol2(bool tnf) { root->Find("mixamorig:RightHand")->collider->colOnOff = tnf; }
	void	  SetAktCol3(bool tnf) { root->Find("mixamorig:RightFoot")->collider->colOnOff = tnf; }
	void	  SetHitCol(bool tnf) { root->Find("mixamorig:Hips")->collider->colOnOff = tnf; }
	
	// �ݶ��̴� On����
	void	  ColOn();
	float	  coliderzerotime = 0.0f; // �ݶ��̴� �� Ÿ�̸ӿ�

	// Monster�� ���� ��ӵ� �����Լ� ������
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

	// �ǰ� ���� �Լ�
	void GetDameged(int damage);

	// ���ݷ� ���� �Լ�
	int	 GetPower() { return power; }
	void SetPower();
};

