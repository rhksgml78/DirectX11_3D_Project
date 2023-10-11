#pragma once
class Camera : public Actor
{
private:
	static ID3D11Buffer* VPBuffer;
	static ID3D11Buffer* VBuffer;
	static ID3D11Buffer* PBuffer;
	static ID3D11Buffer* viewPosBuffer;
	static ID3D11Buffer* viewUpBuffer;
	static ID3D11Buffer* viewForwardBuffer;
	
public:
	static Camera* main;
	static float mainCamSpeed;
public:
	static void CreateStaticMember();
	static void DeleteStaticMember();
	static void ControlMainCam();
	static Camera* Create(string name = "Camera");
private:
	
public:
	Matrix		view, proj;
	Viewport	viewport;
	bool		ortho;
	float		fov,nearZ,farZ, width, height;
protected:
	Camera();
	~Camera();
public:
	void	Update() override;
	void	Set();
	void	SetShadow();
	void	RenderDetail();
	//카메라의 프러스텀과 충돌
	bool    Intersect(Vector3 coord); //중심점
	bool    Intersect(Collider* target); //컬라이더
};

