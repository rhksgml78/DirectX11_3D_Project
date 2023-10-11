#pragma once
enum class Command_State
{
	Success,
	Failure,
	Running
};

class Command
{
protected:
	// �ش� Ŀ�ǵ尡 ��ȿ����(���� ���, Ÿ�� �� ������ �ּҰ� �����Ǿ� �ִٴ���)
	bool isActive = false;

public:
	virtual ~Command() {  };

	virtual		 Command_State Excute() = 0;

	// AI �۵��� ���� �Լ���, Update���� Blackboard�� ������ �����͸� ��������, LateUpdate���� ���忡 ���� �����Ѵ�.

	virtual		 void Update(shared_ptr<class BlackBoard> board) = 0;
	virtual		 void LateUpdate(shared_ptr<class BlackBoard> board) = 0;
	bool 			  GetState() const { return isActive; }


};

