#pragma once
class SKH_TestScene : public Scene
{
private:
	//Grid* grid;
	Terrain* map;

	Camera* cam;

	MonsterBoss* boss;

	Actor* player;
	//APlayer* player;
	float temp = 0.0f;

	MonsterType1* monster[MONSTERNUM];
	MonsterType1* monsterClone;
	MonsterType2* monster2[MONSTERNUM];

	bool	isintro;
	int		introcount = 0;
	float	camtimer = 0;



	GameUI* gameui;

	// 그림자
	Shadow* shadow;
	//Shadow* shadow2;

public:
	SKH_TestScene();
	~SKH_TestScene();
	virtual void Init() override;
	virtual void Release() override; //해제
	virtual void Update() override;
	virtual void LateUpdate() override;//갱신
	virtual void Render() override;
	virtual void PreRender() override;
	virtual void ResizeScreen() override;

	void move();
	void camAction(); // CreateLookAt함수를 사용한 카메라시점변경및 움직임 구현
	void camCheck();
	void lookAtrotation(); // CreateLookAt함수를 사용한 플레이어의 3D 각도 변경 구현
};

