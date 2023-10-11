#pragma once
class Scene1 : public Scene
{
private:
	Camera* cam;
	Sky* sky;
	Terrain* map;
	Environment* cube;



public:
	Scene1();
	~Scene1();
	virtual void Init() override;
	virtual void Release() override; //해제
	virtual void Update() override;
	virtual void LateUpdate() override;//갱신
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void ResizeScreen() override;
};

