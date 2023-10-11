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
	// 해당 커맨드가 유효한지(예를 들면, 타겟 및 본인의 주소가 설정되어 있다던가)
	bool isActive = false;

public:
	virtual ~Command() {  };

	virtual		 Command_State Excute() = 0;

	// AI 작동을 위한 함수들, Update에서 Blackboard에 접근해 데이터를 가져오고, LateUpdate에서 보드에 값을 갱신한다.

	virtual		 void Update(shared_ptr<class BlackBoard> board) = 0;
	virtual		 void LateUpdate(shared_ptr<class BlackBoard> board) = 0;
	bool 			  GetState() const { return isActive; }


};

