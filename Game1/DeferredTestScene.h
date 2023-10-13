#pragma once
class DeferredTestScene : public Scene
{
private:
	Camera* cam;

	Actor* player;
	Actor* playerClone[10];

	Deferred* deferred;
	Actor* post;

public:
	DeferredTestScene();
	~DeferredTestScene();
	virtual void Init() override;
	virtual void Release() override; //����
	virtual void Update() override;
	virtual void LateUpdate() override;//����
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void ResizeScreen() override;
};

