#pragma once

class RocksEffectVt
{
private:
	bool isMoveUp = true;
	bool isActive = false;
	// 스킬이펙트가 최하로 내려갈 위치
	float minpos = -50;
	// 스킬 이펙트가 최대로 올라갈 위치
	float maxpos = 10;

	// 스킬 이펙트의 내려가는 속도
	float downspeed = 40;
	// 스킬 이펙트의 올라가는 속도
	float upspeed = 300;

	bool isPop = true;

public:
	RocksEffectVt();
	~RocksEffectVt();
	Actor* root;
	Actor* rocks[MAXROCK];
	ExploedEffect* pop[MAXROCK];
	// 타이머에 사용할 변수
	float			skillcountzero = 0.0f;
	int				skillcount = 0;
	bool			isskill[MAXROCK] = {};
	bool			isallfalse = false;
	bool			islast = false;
	float			g = 0.0f;

	// 스킬 이미시브 컨트롤용 타이머 변수
	float emzrotime = 0.0f;

	void Init();
	void Release();
	void Update();
	void Render();
	void RenderHierarchy();

	void Action();
	void EmissiveChange();

	// 위치 세팅
	void Setminpos(float minpos) { this->minpos = minpos; }
	void Setmaxpos(float maxpos) { this->maxpos = maxpos; }

	// 속도 세팅
	void SetUpSpeed(float upspeed) { this->upspeed = upspeed; }
	void SetDownSpeed(float downspeed) { this->downspeed = downspeed; }

	// 실행 여부 세팅
	void SetActive(bool isActive) { this->isActive = isActive; }
	
	// 설정값 반환
	float GetMinPos() { return minpos; }
	float GetMaxPos() { return maxpos; }
};

