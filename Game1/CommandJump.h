#pragma once
class CommandJump : public Command
{
private:
	Actor* actor;
	function<bool(Terrain&, Ray, Vector3&)> computePicking;

	Terrain* terrain;

	bool isJump = false;

	float gravity = 0.0f;
	//
private:
	Command_State Jump();

public:
	CommandJump() {}
	CommandJump(Actor* actor)
		:actor(actor)
	{}
	~CommandJump() {}

	// Command을(를) 통해 상속됨
	virtual Command_State Excute() override;
	virtual void Update(shared_ptr<BlackBoard> board) override;
	virtual void LateUpdate(shared_ptr<BlackBoard> board) override;

	void SetTerrain(Terrain* terrain) { this->terrain = terrain; }
	void CheckState();
};

