#pragma once
class BlendState :public Singleton<BlendState>
{
	ID3D11BlendState* Enable; //���� ����
	ID3D11BlendState* Unable; //���� �׽�Ʈ
public:
	BlendState();
	~BlendState();
	void Set(bool BlendEnable);
};
