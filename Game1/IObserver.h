#pragma once
class IObserver
{
	// ������ �������̽��� ��� ���� ���� ���������� �����ϱ� ���� Ŭ����
	// ���� ���������� Ư�� ����(Subject)�� �����ϰ� � �̺�Ʈ�� �߻��ϸ� �������� Notify�� ���� Update ����

public:
	string name;

public:
	virtual ~IObserver();
	virtual void Update() = 0;
};

