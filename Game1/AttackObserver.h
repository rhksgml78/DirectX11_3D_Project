#pragma once
class AttackObserver : public IObserver
{
private:
	// �ǰ� ���� �� ���ΰ� �´��� Ȯ���� �̸�
	vector<GameObject*> col;
	//class Character* character;
	Actor* root;

	bool isHit;
	float timer;

	float atk;

public:
	AttackObserver(Actor* root)
		: root(root)
	{
	};
	AttackObserver();
	~AttackObserver();

public:

	// IObserver��(��) ���� ��ӵ�
	virtual void Update() override;

	void	SetCol(vector<GameObject*>& col) { this->col = col; }
	void	SetHit(bool ishit) { this->isHit = ishit; }
	void	SetAtk(float atk) { this->atk = atk; }

	bool	GetHit() { return isHit; }
	float	GetAtk() { return atk; }

	void TurnOnHit() { isHit = !isHit; }
};

