#pragma once
class ISubject
{
	// 주제(Subject)는 옵저버들의 리스트를 가진다.
	// 자신을 관찰하는 옵저버들에게 특정 이벤트가 발생할 경우 전체 알림(Notify)를 날린다.

protected:
	std::map<string, IObserver*> obsList;

public:
	ISubject();
	virtual ~ISubject();

	virtual void AddObserver(IObserver* obs) { obsList.insert(pair<string, IObserver*>(obs->name, obs)); }
	virtual void PopObserver(IObserver* obs) { obsList.erase(obs->name); }
	virtual void Notify() = 0;
};
