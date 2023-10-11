#pragma once
class BossScene : public Scene
{
private:
	// 구성
	Camera* cam;
	Sky* sky;
	Shadow* shadow;
	Terrain* map;
	Actor* map_stone[3];

	// Actor (Player & Boss)
	MonsterBoss* boss;
	APlayer* player; // 플레이어 대체 액터

	MonsterType1* monster;
	MonsterType2* monster2;
	
	GameUI* gameui;

	// 컷씬 관련
	bool	isintro;
	int		introcount = 0;
	float	camtimer = 0;
	bool	iscutscene = false;

public:
	BossScene();
	~BossScene();
	virtual void Init() override;
	virtual void Release() override; //해제
	virtual void Update() override;
	virtual void LateUpdate() override;//갱신
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void ResizeScreen() override;
	void Hi();
	void CamSetPlayer();
	bool CamisPlayer = true;

	// 컷씬 & 화면전환 모션
	void CutScene();
	void camAction(); // CreateLookAt함수를 사용한 카메라시점변경및 움직임 구현
	void camCheck();

	// 충돌 관련
	bool PlayerAttaks();
	bool BossAttacks();
	bool BossSkillAttacks();
	bool MonsterAttacks();
	bool Monster2Attacks();

	// 그림자 적용 쉐이더로드 함수
	void ShadowShaderSet();
};
