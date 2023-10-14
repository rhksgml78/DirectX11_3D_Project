#pragma once
class MonsterType2 : public Monster
{
public:
	enum class Monster_State
	{
		NONE,
		STANDBY,
		NORMAL,
		HIT,
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
	bool isskill = true;

	bool is_hit = false;

	Vector3 lastPos;

	// �ൿƮ�� ����
	BehaviorTree* BT;

	// ����Ʈ
	IcicleEffect* iceeffect;
	FireEffect* fireeffect;
	BloodEffect* bloodeffect;
	DeathEffect* deatheffect;
	Actor* FireCol;
	Actor* IcicleCol;

	// �ݶ��̴� ����
	Collider* AtkCol1;
	Collider* AtkCol2;
	Collider* HitCol;

public:
	int		hp = 10;
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
	Collider* GetAktCol1() { return this->FireCol->collider; }
	Collider* GetAktCol2() { return this->IcicleCol->collider; }
	Collider* GetHitCol() { return HitCol; }
	void	  SetAktCol1(bool tnf) { FireCol->root->collider->colOnOff = tnf; }
	void	  SetAktCol2(bool tnf) { IcicleCol->root->collider->colOnOff = tnf; }
	void	  SetHitCol(bool tnf) { root->Find("mixamorig:Hips")->collider->colOnOff = tnf; }
	
	// �ݶ��̴� ON����
	void	  ColOn();
	float	  coliderzerotime = 0.0f; // �ݶ��̴� �� Ÿ�̸ӿ�

	// Monster�� ���� ��ӵ� �����Լ� ������
	virtual void Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void RenderHierarchy() override;

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
};

