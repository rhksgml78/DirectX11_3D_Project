#pragma once
class ISubject
{
	// ����(Subject)�� ���������� ����Ʈ�� ������.
	// �ڽ��� �����ϴ� �������鿡�� Ư�� �̺�Ʈ�� �߻��� ��� ��ü �˸�(Notify)�� ������.

protected:
	std::map<string, IObserver*> obsList;

public:
	ISubject();
	virtual ~ISubject();

	virtual void AddObserver(IObserver* obs) { obsList.insert(pair<string, IObserver*>(obs->name, obs)); }
	virtual void PopObserver(IObserver* obs) { obsList.erase(obs->name); }
	virtual void Notify() = 0;
};
