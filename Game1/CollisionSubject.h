#pragma once
class CollisionSubject : public ISubject
{
private:
	vector<Collider*> colList;

public:
	CollisionSubject() {}
	virtual ~CollisionSubject() {}

public:
	void SetList(vector<Collider*> colList) { this->colList = colList; }

public:
	// ISubject을(를) 통해 상속됨
	virtual void Notify() override;
};
