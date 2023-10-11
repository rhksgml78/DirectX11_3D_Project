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
	// ISubject��(��) ���� ��ӵ�
	virtual void Notify() override;
};
