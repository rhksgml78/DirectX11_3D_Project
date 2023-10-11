#pragma once
#define SIZE 10

class StudyScene : public Scene
{
private:
	Camera* cam;
	Sky* sky;
	Terrain* map;
	Actor* player;
	Actor* sphere;
	class Inven* inven;

	// 그림자
	Shadow* shadow;

	// 인스턴싱
#pragma region instuncing
	//Actor* actor2[SIZE][SIZE][SIZE];
	//Actor* actorInstance;
#pragma endregion

	// 물효과 Water_Effect
#pragma region Water_Effect
	Actor* water;

	struct WaterBuffer
	{
		Vector2 dir;
		float time;
		float padding;
	}waterBufferDesc;

	Environment* envi;
	ID3D11Buffer* waterBuffer;
#pragma endregion

public:
	StudyScene();
	~StudyScene();
	virtual void Init() override;
	virtual void Release() override; //해제
	virtual void Update() override;
	virtual void LateUpdate() override;//갱신
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void ResizeScreen() override;
};

