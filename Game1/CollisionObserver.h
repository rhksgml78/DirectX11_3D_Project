#pragma once
class CollisionObserver : public IObserver
{
private:
	Collider* col;
	vector<Collider*> colList;

public:
	bool isCollision;

public:
	CollisionObserver() {}
	CollisionObserver(Collider* col)
		:col(col)
	{}
	virtual ~CollisionObserver() {}

public:
	void SetCol(Collider* col) { this->col = col; }
	void SetList(vector<Collider*> colList) { this->colList = colList; }

public:
	// IObserver��(��) ���� ��ӵ�
	virtual void Update() override;
};

