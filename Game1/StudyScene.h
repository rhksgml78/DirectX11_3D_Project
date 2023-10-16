#pragma once
#define SIZE 10

class StudyScene : public Scene
{
private:
	Camera* cam;
	Sky* sky;
	Terrain* map;
	Actor* player;
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
		float scale;
		float refractscalar;
		Vector3 padding;
	}waterBufferDesc;

	Environment* envi;
	Environment* envi2;
	ID3D11Buffer* waterBuffer;
	ID3D11Buffer* planeBuffer;	// 평면 방정식에 사용할 버퍼가 필요! 23.10.12 추가
	Vector4 plane; // 평면 23.10.12 추가
	float waterspeed = 1.0f;
	Vector2 waterdir{1,1};
	float refractscalar = 0.003f;
	float uvscale = 1.0f;
	bool axistnf = true;
	Vector3 rttpad = {0,0,1};

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

