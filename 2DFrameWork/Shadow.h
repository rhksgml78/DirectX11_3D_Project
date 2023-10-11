#pragma once
class Shadow : public RenderTarget
{
	struct ShadowDesc
	{
		int		ShadowQuality = 1;
		float	ShadowBias = 0.00325f;
		Vector2 Size = Vector2(0.1f, 0.1f);
	}desc;
	static ID3D11Buffer* shadowPSBuffer;
	static ID3D11Buffer* shadowVSBuffer;
	Camera* cam;
	float				textureSize;
public:
	float				range;
	static void CreateStaticMember();
	static void DeleteStaticMember();

	Shadow();
	~Shadow();

	void SetTarget(Vector3 target);
	void SetRGBTexture(int slot);
	void ResizeScreen(float width, float height);
	void RenderDetail();

};

