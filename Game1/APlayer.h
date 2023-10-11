#pragma once

/*
플레이어 콜라이더 위치
Hit 콜라이더 : mixamorig:Hips
Attack 콜라이더 : AttackCol
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
	// 무기변경용 변수
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

	// 공격 속도
	float low_speed = 1.0f;
	float middle_speed = 1.3f;
	float high_speed = 1.7f;

	// 콜라이더
	Collider* AtkCol1;
	Collider* HitCol;

public:
	bool		camLock;
	Vector3 lastPos;
	Actor* root;

	// 그림자적용시키려고 public으로 이동시켰습니다.
	Actor* sword; //오른손 검
	Actor* axe; // 도끼
	Actor* armor; // 갑옷
	Actor* Lefthand; //왼손 방패??

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

	// 콜라이더
	Collider* GetAktCol1() { return AtkCol1; }
	Collider* GetHitCol() { return HitCol; }

	// 타겟 지정
	void SetTarget(Actor* target) {this->lock = target; }
	void CollOn();
	float collonzerotime = 0.0f;
	float collonzerotime2 = 0.0f;

	void GetDameged(int damage); //플레이어의 공격력 만큼 데미지 받기
	bool isDameged = false;
	float Damegedzerotime = 0.0f;

	// 공격력 관련 함수
	int	GetPower() { return power; }
	void SetPower();
	
	// 애니매이션 한번에 한번만 때릴수있도록 공격용 콜라이더 세팅
	void SetAtkCol();
};