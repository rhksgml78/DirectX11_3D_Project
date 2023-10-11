#include "Solar.h"
#include "GameUI.h"
#pragma once


class Scene2 : public Scene
{
private:
	Sky* sky;

	GameUI* gameui;

	Camera*		cam;

	APlayer* player;


	// 몬스터
	MonsterType1* monster[MONSTERNUM];
	MonsterType1* monster3[MONSTERNUM];
	MonsterType2* monster2[MONSTERNUM];

	int monCount1 = MONSTERNUM;
	int monCount2 = MONSTERNUM;
	int monCount3 = MONSTERNUM;

	// 그림자
	Shadow* shadow;

public:
	Map* map;

	Scene2();
	~Scene2();
	virtual void Init() override;
	virtual void Release() override; //해제
	virtual void Update() override;
	virtual void LateUpdate() override;//갱신
	virtual void Render() override;
	virtual void PreRender() override;
	virtual void ResizeScreen() override;

	//void CameraMove1();
	void CamSetPlayer();
	bool CamisPlayer = true;


	// 충돌 관련
	bool PlayerAttaks();
	bool MonsterAttacks();
	bool Monster2Attacks();
};

