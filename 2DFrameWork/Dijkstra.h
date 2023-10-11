#pragma once

enum class DiNode_TYPE
{
    ORIGINAL,
    PATHNODE
};

class DiNode
{
    struct DiEdge
    {
        float cost;
        bool  isActive;

    } _DiEdge;

    friend class Di;
private:
    int             id;
    bool            find;   //�˻����� �ִ°�?
    int             prev;   //���� ���ŵ� ���


    //              key   cost
public:
    DiNode_TYPE diType;

    unordered_map< int, DiEdge> linkedList;
    Vector3         pos;
    float     cost;   //������� ���
    DiNode() { }
    DiNode(int id) { this->id = id; }
    void Link(int node, float cost)
    {
        linkedList[node].cost = cost;
        linkedList[node].isActive = true;
    }
    void Reset()
    {
        prev = 0;
        cost = FLT_MAX;
        find = false;
    }

    int GetId() { return id; }

};

class DiNodeCompare
{
public:
    bool operator()(DiNode*& a, DiNode*& b)
    {
        return a->cost > b->cost;
    }
};

class Di
{

    //�ߺ����� Ű, ��������
       //            key   node
public:
    Di()
    {}

    unordered_map< int, DiNode*> list;
    string                      file = " ";

    int GetNode(Vector3 pos);


    DiNode*& AddNode(Vector3 pos);

    bool PopNode(int str);


    void LinkNode(int str1, int str2);


    bool PathFinding(int from, int to, std::list<Vector3>& way);

    void SaveFile(string file);

    void LoadFile(string file);

};

