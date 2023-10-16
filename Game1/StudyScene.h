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

	// �׸���
	Shadow* shadow;

	// �ν��Ͻ�
#pragma region instuncing
	//Actor* actor2[SIZE][SIZE][SIZE];
	//Actor* actorInstance;
#pragma endregion

	// ��ȿ�� Water_Effect
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
	ID3D11Buffer* planeBuffer;	// ��� �����Ŀ� ����� ���۰� �ʿ�! 23.10.12 �߰�
	Vector4 plane; // ��� 23.10.12 �߰�
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
	virtual void Release() override; //����
	virtual void Update() override;
	virtual void LateUpdate() override;//����
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void ResizeScreen() override;
};

