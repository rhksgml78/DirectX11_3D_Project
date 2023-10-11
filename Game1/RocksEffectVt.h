#pragma once

class RocksEffectVt
{
private:
	bool isMoveUp = true;
	bool isActive = false;
	// ��ų����Ʈ�� ���Ϸ� ������ ��ġ
	float minpos = -50;
	// ��ų ����Ʈ�� �ִ�� �ö� ��ġ
	float maxpos = 10;

	// ��ų ����Ʈ�� �������� �ӵ�
	float downspeed = 40;
	// ��ų ����Ʈ�� �ö󰡴� �ӵ�
	float upspeed = 300;

	bool isPop = true;

public:
	RocksEffectVt();
	~RocksEffectVt();
	Actor* root;
	Actor* rocks[MAXROCK];
	ExploedEffect* pop[MAXROCK];
	// Ÿ�̸ӿ� ����� ����
	float			skillcountzero = 0.0f;
	int				skillcount = 0;
	bool			isskill[MAXROCK] = {};
	bool			isallfalse = false;
	bool			islast = false;
	float			g = 0.0f;

	// ��ų �̹̽ú� ��Ʈ�ѿ� Ÿ�̸� ����
	float emzrotime = 0.0f;

	void Init();
	void Release();
	void Update();
	void Render();
	void RenderHierarchy();

	void Action();
	void EmissiveChange();

	// ��ġ ����
	void Setminpos(float minpos) { this->minpos = minpos; }
	void Setmaxpos(float maxpos) { this->maxpos = maxpos; }

	// �ӵ� ����
	void SetUpSpeed(float upspeed) { this->upspeed = upspeed; }
	void SetDownSpeed(float downspeed) { this->downspeed = downspeed; }

	// ���� ���� ����
	void SetActive(bool isActive) { this->isActive = isActive; }
	
	// ������ ��ȯ
	float GetMinPos() { return minpos; }
	float GetMaxPos() { return maxpos; }
};

