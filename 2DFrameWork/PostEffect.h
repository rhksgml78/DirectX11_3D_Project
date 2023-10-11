#pragma once
class PostEffect
{
	struct PSP
	{
		int filter = 0;
		Vector3 color;
		Vector2 screen; //ÁÂÇ¥
		float radius = 100;
		int count = 1;

		int select = 0;
		float width;
		float height;
		float padding;
	};
	ID3D11Buffer* PEBuffer;
	
	RenderTarget* RTT;
	Actor* postEffect;
public:
	PSP psp;
	PostEffect();
	~PostEffect();

	void Update();
	void SetTarget();
	void Render();
	void RenderDetail();
};

