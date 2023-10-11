#pragma once
class Monster
{

public:
	// 가상 함수들
	virtual void Init() = 0;
	virtual void Release() = 0;
	virtual void Update() = 0;
	virtual void LateUpdate() = 0;
	virtual void Render() = 0;
	virtual void RenderHierarchy() = 0;
};

