#pragma once

class GItem
{
    friend class GInven;
public:
    string  name = "";      //�̸�
    string  file;           //����� �ؽ�ó
    string  xmlfile;        //����� xml
    int     type;           //Ÿ��
    int     listidx = -1;   //�����۸���Ʈ������ �ε�����ȣ

    int     weight = 0;     //����
    int     nums;           //����(ui�ʿ��� ����Ҷ� ����� �ȹ޾����� ������ ��¦ ���� num������ �ʹ� ���Ƽ� ���ε�)
    int     detail;         //��,���� ��(�ؿ� ����Ŭ�������� ����ؾ��ϴµ� ���ؼ� �ϴ� �����Ϸ� ��ü)
};

class GWeapon : public GItem
{
public:
    friend class GInven;
    int att;        //���ݷ�

};

class Helmet : public GItem
{
public:
    friend class GInven;
    int df;        //����

};

class Armor : public GItem
{
public:
    friend class GInven;
    int df;        //����

};

class Glove : public GItem
{
public:
    friend class GInven;
    int df;        //����

};

class Boots : public GItem
{
public:
    friend class GInven;
    int df;        //����

};

class Useable : public GItem
{
public:
    friend class GInven;
    int num;        //����

};

//�׷��� ��ü , ���ο� ������ �����͸� ����
class GInven : public UI
{

public:
    //txt ���� �о�� ������ ����Ʈ
    static GItem** itemList;
    static int      itemListSize;

    // ���Կ����� �������� �������� ���� �����Ϸ��� �� ���·� ��������� ���� �׷����ʰ� 
    // ���Կ� ��ȣ�� �����Ų �� �ش� ��ȣ�� ������
    // ������ ����Ʈ���� ��ȣ�� �ش��ϴ� �������� �ҷ��� ����ϴ� ������� ����� (�ڵ� ����ȭ)
    // -> int�� �迭�� ����ϴ°� ����� ����� rightSlot[0]->listidx Ȥ�� armorSlot->listidx �� ���������
    // int�迭�̾��ٸ� �ٷ� rightSlot[0]�� armorSlot ������ �ش� ������ ��ȣ �ҷ����� ����
    // (���ݴ� �ð��� �����ɷ��� �����Ƽ� ���ٲ���)

    // �������� ���� �������� ���������� Ȯ���� �� �ִ� ������ �ϳ� ������ ������
    
    GItem* rightSlot[3];
    int    rightSlotSize = 3;
    GItem* leftSlot[3];
    int    leftSlotSize = 3;

    GItem* armorSlot;
    GItem* helmetSlot;
    GItem* gloveSlot;
    GItem* bootsSlot;

    GItem* useableSlot[5];
    int    useableSlotSize = 5;
    int    useableidx = -1; // ����� ���������� �ε��� ��ȣ�� ������ ����
    GItem* skillSlot[5];
    int    skillSlotSize = 5;

    class Insideinven* sideinven;

    const std::vector<std::string> slotNames =
    {
        "Right0", "Right1", "Right2",
        "Left0", "Left1", "Left2",
        "Armor", "Helmet", "Glove", "Boots",
        "Useable0", "Useable1", "Useable2", "Useable3", "Useable4"
    };
    const std::vector<std::string> subslotNames =
    {
        "Wslot0", "Wslot1", "Wslot2","Wslot3","Wslot4","Wslot5","Wslot6","Wslot7","Wslot8","Wslot9",
        "Hslot0", "Hslot1", "Hslot2","Hslot3","Hslot4","Hslot5","Hslot6","Hslot7","Hslot8","Hslot9",
        "Aslot0", "Aslot1", "Aslot2","Aslot3","Aslot4","Aslot5","Aslot6","Aslot7","Aslot8","Aslot9",
        "Gslot0", "Gslot1", "Gslot2","Gslot3","Gslot4","Gslot5","Gslot6","Gslot7","Gslot8","Gslot9",
        "Bslot0", "Bslot1", "Bslot2","Bslot3","Bslot4","Bslot5","Bslot6","Bslot7","Bslot8","Bslot9",
        "Uslot0", "Uslot1", "Uslot2","Uslot3","Uslot4","Uslot5","Uslot6","Uslot7","Uslot8","Uslot9",
        "Sslot0", "Sslot1", "Sslot2","Sslot3","Sslot4","Sslot5","Sslot6","Sslot7","Sslot8","Sslot9"
    };

    bool onoff;

    static void CreateStaticMember();
    static void DeleteStaticMember();

    static GInven* Create(string name = "Inven");
    //void	RenderDetail();
    void LoadInven();
    void SaveInven();
    void Render();
    void Update();
};

class Insideinven : public UI
{
    const int numofitem = 10;
public:
    GItem* Waeponinven[10];
    int    waeponidx = 0;
    GItem* Helmetinven[10];
    int    helmetidx = 0;
    GItem* Armorinven[10];
    int    armoridx = 0;
    GItem* Gloveinven[10];
    int    gloveidx = 0;
    GItem* Bootsinven[10];
    int    bootsidx = 0;
    GItem* Useableinven[10];
    int    useableidx = 0;


    static Insideinven* Create(string name = "InsideInven");

    void LoadInven();
    void SaveInven();
    void Render();
    void Update();
    void Open();
};