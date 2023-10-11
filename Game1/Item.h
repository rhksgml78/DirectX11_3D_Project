#pragma once

class Item
{
    friend class Inven;
    int    type;    //Ÿ��
    string name;    //�̸�
    int    num;     //����
    string file;    //����� �ؽ�ó or xml

};

class Weapon : public Item
{
public:
    friend class Inven;
    int att;        //���ݷ�

};

//�׷��� ��ü , ���ο� ������ �����͸� ����
class Inven : public UI
{
    //txt ���� �о�� ������ ����Ʈ
    static Item**   itemList;
    static int      itemListSize;

    //���߹迭�� ���� ������
    Item*           itemSlot[2][2];
    int             itemSlotSize;
    /*enum class MouseState
    {
        NONE,
        CL,
    };*/
    //���콺 ��ġ�� �׷��� ��ü
    UI* mouse;
    int select;
    Int2 clickIdx;
public:

    static void CreateStaticMember();
    static void DeleteStaticMember();

    static Inven* Create(string name = "UI");
    //void	RenderDetail();
    void LoadInven();
    void SaveInven();
    void Render();
    void Update();


};