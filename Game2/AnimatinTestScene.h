#pragma once
class AnimatinTestScene : public Scene
{
private:
	Camera* Cam;
	Camera* Cam2;

	Grid* grid;

	Actor* test;
	SlashTrail* boss_trail;

	Actor* test2;

	int state;
	int fontsize;
	RECT fontrect;
	wstring fontstyle;

	bool roar;
	bool idle;
	bool walk;
	bool run;
	bool jump;
	bool attack1;
	bool attack2;
	bool attack3;
	bool death;
	bool parry;
	bool parryfail;

public:
	AnimatinTestScene();
	~AnimatinTestScene();
	virtual void Init() override;
	virtual void Release() override; //해제
	virtual void Update() override;
	virtual void LateUpdate() override;//갱신
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void ResizeScreen() override;
	void statealllfalse();
	void renderText();
};

