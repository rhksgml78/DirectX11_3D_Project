#pragma once
class BossScene : public Scene
{
private:
	// ����
	Camera* cam;
	Sky* sky;
	Shadow* shadow;
	Terrain* map;
	Actor* map_stone[3];

	// Actor (Player & Boss)
	MonsterBoss* boss;
	APlayer* player; // �÷��̾� ��ü ����

	MonsterType1* monster;
	MonsterType2* monster2;
	
	GameUI* gameui;

	// �ƾ� ����
	bool	isintro;
	int		introcount = 0;
	float	camtimer = 0;
	bool	iscutscene = false;

public:
	BossScene();
	~BossScene();
	virtual void Init() override;
	virtual void Release() override; //����
	virtual void Update() override;
	virtual void LateUpdate() override;//����
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void ResizeScreen() override;
	void Hi();
	void CamSetPlayer();
	bool CamisPlayer = true;

	// �ƾ� & ȭ����ȯ ���
	void CutScene();
	void camAction(); // CreateLookAt�Լ��� ����� ī�޶��������� ������ ����
	void camCheck();

	// �浹 ����
	bool PlayerAttaks();
	bool BossAttacks();
	bool BossSkillAttacks();
	bool MonsterAttacks();
	bool Monster2Attacks();

	// �׸��� ���� ���̴��ε� �Լ�
	void ShadowShaderSet();
};
