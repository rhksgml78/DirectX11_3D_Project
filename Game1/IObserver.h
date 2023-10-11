#pragma once
class IObserver
{
	// 옵저버 인터페이스는 상속 받은 하위 옵저버들을 관리하기 위한 클래스
	// 하위 옵저버들은 특정 주제(Subject)를 관찰하고 어떤 이벤트가 발생하면 주제들의 Notify를 통해 Update 진행

public:
	string name;

public:
	virtual ~IObserver();
	virtual void Update() = 0;
};

