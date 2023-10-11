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

	// �׸���
	Shadow* shadow;
	//Shadow* shadow2;

public:
	SKH_TestScene();
	~SKH_TestScene();
	virtual void Init() override;
	virtual void Release() override; //����
	virtual void Update() override;
	virtual void LateUpdate() override;//����
	virtual void Render() override;
	virtual void PreRender() override;
	virtual void ResizeScreen() override;

	void move();
	void camAction(); // CreateLookAt�Լ��� ����� ī�޶��������� ������ ����
	void camCheck();
	void lookAtrotation(); // CreateLookAt�Լ��� ����� �÷��̾��� 3D ���� ���� ����
};

