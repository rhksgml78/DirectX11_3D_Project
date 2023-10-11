#pragma once
class AttackSubject : public ISubject
{
private:
	vector<GameObject*> col;

	float atk;
public:
	AttackSubject();
	~AttackSubject();

public:
	// ISubject을(를) 통해 상속됨
	virtual void Notify() override;

	void SetAtk(float atk) { this->atk = atk; }

	void SetCol(GameObject* col)
	{
		bool tf = false;

		for (auto it : this->col)
		{
			if (it == col)
				tf = true;
		}

		if (!tf)
		{
			this->col.push_back(col);
		}
	}

	float GetAtk() { return atk; }
};

