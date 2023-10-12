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
		float padding;
	}waterBufferDesc;

	Environment* envi;
	Environment* envi2;
	ID3D11Buffer* waterBuffer;
	ID3D11Buffer* planeBuffer;	// ��� �����Ŀ� ����� ���۰� �ʿ�! 23.10.12 �߰�
	Vector4 plane; // ��� 23.10.12 �߰�

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

