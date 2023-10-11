#pragma once

/*
�÷��̾� �ݶ��̴� ��ġ
Hit �ݶ��̴� : mixamorig:Hips
Attack �ݶ��̴� : AttackCol
*/
enum class AnimState
{
	WALK,
	STRONG_ATTACK,
	START_BLOCK,
	SIDE_WALK_RIGHT,
	SIDE_WALK_LEFT,
	RUN_FRONT,
	RIGHT_WALK,
	NORMAL_ATTACK,
	LEFT_WALK,
	JUMP_STAND,
	JUMP_RUN,
	HIT2,
	HIT1,
	FRONT_MAGIC_CAST,
	FLOOR_MAGIC_CAST,
	DRINKING,
	DEATH1,
	DEATH2,
	BLOCK,
	BLOCK_SUCCESS,
	BIG_SIDE_ATTACK,
	COMBO,
	REVERSE_TURN,
	IDLE,
	IDLE1,
	IDLE2,
	Empty,
	ROLL,
	SECOND_ATTACK,
	THIRD_ATTACK,
	EQUIP,
	JUMP_ATTACK,
	SECOND_WEAPON_FIRST_ATTACK,
	SECOND_WEAPON_SECOND_ATTACK,
	SECOND_WEAPON_THIRD_ATTACK,
	SECOND_WEAPON_STRONG_ATTACK,
	WALK_BACK
};

enum class WeaponState
{
	Sword,
	Axe
};


class APlayer
{
	//Camera*		Cam;
	Actor*		player;
	Actor*		lock;
	
	SlashTrail* trail;
	bool		trailEffect;


	WeaponState	weapon;

	AnimState	state;
	AnimState	prevState;

	Vector3		inputDir;
	Vector3		tempPos;
	Vector3		rollDirection;

	float		speed;
	float		gravity;
	float		roolTimer;
	float		secondComboTimer;
	float		thirdComboTimer;
	float		dist;
	float		rotationTimer;
	float		roolInterpolationTimer;
	float		lerpAmount;

	bool		isLanding;
	bool		isRunning;
	bool		secondComboAttack;
	bool		thirdComboAttack;
	bool		roolInput;
	bool		lerpAfterRoll;
	// ���⺯��� ����
	bool		isWeaponCange = false;

	const float FORWARD_SPEED = 4000.0f;
	const float GRAVITY_ACCELERATION = 11.0f;
	const float ROLL_SPEED = 48.0f;

	int		hp = 100;
	int		maxhp = hp;
	int		mp = 50;
	int		maxmp = mp;
	int		stamina = 20;
	int		maxstamina = stamina;
	int		power = 5;

	// ���� �ӵ�
	float low_speed = 1.0f;
	float middle_speed = 1.3f;
	float high_speed = 1.7f;

	// �ݶ��̴�
	Collider* AtkCol1;
	Collider* HitCol;

public:
	bool		camLock;
	Vector3 lastPos;
	Actor* root;

	// �׸��������Ű���� public���� �̵����׽��ϴ�.
	Actor* sword; //������ ��
	Actor* axe; // ����
	Actor* armor; // ����
	Actor* Lefthand; //�޼� ����??

	void Init();
	void Update();
	void LateUpdate();
	void RenderHierarchy();
	void Render();
	void WASD();
	Ray GetRay()
	{
		Ray Top;
		Top.position = root->GetWorldPos() + Vector3(0, 1000, 0);
		Top.direction = Vector3(0, -1, 0);
		return Top;
	}
	void OutOfRange();
	void OnFloor(Vector3 Hit);
	void BackMove() { root->SetWorldPos(lastPos); root->Update(); };
	void Release();
	void AnimationMachine();
	void ChangeRotation();
	void Trail();

	int GetHP() { return hp; }
	void HealHP(int temp);
	void HealMP(int temp);
	int GetMaxHP() { return maxhp; }
	int GetMP() { return mp; }
	int GetMaxMP() { return maxmp; }
	int GetStamina() { return stamina; }
	int GetMaxStamina() { return maxstamina; }

	void Set_Rhandfile(string file);
	void Set_Lhandfile(string file);
	void SetArmorfile(string file);

	// �ݶ��̴�
	Collider* GetAktCol1() { return AtkCol1; }
	Collider* GetHitCol() { return HitCol; }

	// Ÿ�� ����
	void SetTarget(Actor* target) {this->lock = target; }
	void CollOn();
	float collonzerotime = 0.0f;
	float collonzerotime2 = 0.0f;

	void GetDameged(int damage); //�÷��̾��� ���ݷ� ��ŭ ������ �ޱ�
	bool isDameged = false;
	float Damegedzerotime = 0.0f;

	// ���ݷ� ���� �Լ�
	int	GetPower() { return power; }
	void SetPower();
	
	// �ִϸ��̼� �ѹ��� �ѹ��� �������ֵ��� ���ݿ� �ݶ��̴� ����
	void SetAtkCol();
};