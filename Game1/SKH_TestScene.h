#pragma once
class SKH_TestScene : public Scene
{
private:
	Camera* cam;
	Camera* cam2;
	Sky* sky;
	Shadow* shadow;
	Terrain* map;

	// Actor (Player & Boss)
	MonsterBoss* boss;
	APlayer* player; // 플레이어 대체 액터
	//Actor* player2; // 플레이어 대체 액터

	MonsterType1* monster;
	MonsterType2* monster2;

	wstring backgroundname = L"Cube Map Style : ";
	float zerotimer = 0.0f;
	int count = 0;
	bool mapchange = false;

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

	void RenderTexture();
	void ChangeCubemap();
	void ShadowDwrite();

};

