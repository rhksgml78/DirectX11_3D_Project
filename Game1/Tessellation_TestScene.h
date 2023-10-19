#pragma once
class Tessellation_TestScene : public Scene
{
private:
	Camera* cam;
	Mesh* mesh;
	Shader* shader;

	struct EdgeBuffer
	{
		Vector3 edges;
		float inside;
	}edgeBufferDesc;

	ID3D11Buffer* edgeBuffer;

public:
	Tessellation_TestScene();
	~Tessellation_TestScene();
	virtual void Init() override;
	virtual void Release() override; //����
	virtual void Update() override;
	virtual void LateUpdate() override;//����
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void ResizeScreen() override;
};

