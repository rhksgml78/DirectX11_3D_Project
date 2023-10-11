#include "stdafx.h"
#include "GameUI.h"

// static �����̱⶧���� ������������ �ʱ�ȭ�� �ѹ� �ؾ��Ѵ�.
GItem** GInven::itemList = nullptr;
int   GInven::itemListSize = 0;

void GInven::CreateStaticMember()
{
    ifstream fin;

    // ���ϼ����ؼ� �ֱ�
    
    // ����� ����� ���ʸ� xml�� �����ϰ� �ִµ� assets�� �����ؼ�
    // loader�� ���� xml������ ������ ���߱� ����
    // �ǹ��� �ִ� �������� �߰��Ҷ� �� assets�� ���ϰ� xml������� Ȯ���� �� �߰�����
    // �׷��� ������ �� �� �ִٸ� �ؿ� xmlfile �����ϴ� �ּ��� Ǯ�� ���ϼ����ϸ� ����ϸ� �ȴ�.
    
    fin.open("../Contents/test.db", ios::in);

    // �޸��屸��
    // �����۰���(������)
    // Ÿ�� ��ȣ ���� �̹����̸� ���� xml���ϸ�
    // Ÿ�� ��ȣ ���� �̹����̸� ���� xml���ϸ�
    // Ÿ�� ��ȣ ���� �̹����̸� ���� xml���ϸ�
    // ...
    // (test.db �������α׷� �޸������� ��� ����, �߰�,������ ���� ����)

    fin >> itemListSize;

    itemList = new GItem * [itemListSize];

    for (int i = 0; i < itemListSize; i++)
    {
        int idx;
        int type;
        fin >> idx; //������ ��ȣ
        fin >> type; //������ Ÿ��
        if (type == 0)
        {      
            itemList[idx] = new GItem();        // ��Ÿ �� (��������x)
            fin >> itemList[idx]->name;
            fin >> itemList[idx]->file;

        }
        else  if (type == 1)
        {
            GWeapon* weapon = new GWeapon();    // ����
            fin >> weapon->name;
            fin >> weapon->file;
            fin >> weapon->detail;
            fin >> weapon->xmlfile;
            itemList[idx] = weapon;
        }
        else  if (type == 2)
        {
            Helmet* helmet = new Helmet();    // �ҹ��
            fin >> helmet->name;
            fin >> helmet->file;
            fin >> helmet->detail;
            //fin >> helmet->xmlfile;
            itemList[idx] = helmet;
        }
        else  if (type == 3)
        {
            Armor* armor = new Armor();       // ����
            fin >> armor->name;
            fin >> armor->file;
            fin >> armor->detail;
            fin >> armor->xmlfile;
            itemList[idx] = armor;
        }
        else  if (type == 4)
        {
            Glove* glove = new Glove();       // �尩
            fin >> glove->name;
            fin >> glove->file;
            fin >> glove->detail;
            //fin >> glove->xmlfile;
            itemList[idx] = glove;
        }
        else  if (type == 5)
        {
            Boots* boots = new Boots();       // ����
            fin >> boots->name;
            fin >> boots->file;
            fin >> boots->detail;
            //fin >> boots->xmlfile;
            itemList[idx] = boots;
        }
        else  if (type == 6)
        {
            Useable* useable = new Useable();       // �����
            fin >> useable->name;
            fin >> useable->file;
            fin >> useable->detail;
            fin >> useable->nums;
            itemList[idx] = useable;
        }
        itemList[idx]->type = type;
        itemList[idx]->listidx = idx;
    }

    for (int i = 0; i < itemListSize; i++)
    {
        cout << i << "��° �������� �̸� : " << itemList[i]->name << endl;
    }
}

void GInven::DeleteStaticMember()
{
    SafeDeleteArray(itemList);
}

GInven* GInven::Create(string name)
{
    GInven* temp = new GInven();
    temp->name = name;
    temp->type = ObType::UI;

    temp->CreateStaticMember();

    temp->LoadFile("Inven.xml");


    for (int i = 0; i < temp->rightSlotSize; i++)
    {
        temp->rightSlot[i] = new GItem();
    }
    for (int i = 0; i < temp->leftSlotSize; i++)
    {
        temp->leftSlot[i] = new GItem();
    }
    for (int i = 0; i < temp->useableSlotSize; i++)
    {
        temp->useableSlot[i] = new GItem();
    }
    for (int i = 0; i < temp->skillSlotSize; i++)
    {
        temp->skillSlot[i] = new GItem();
    }

    temp->armorSlot = new GItem();
    temp->helmetSlot = new GItem();
    temp->gloveSlot = new GItem();
    temp->bootsSlot = new GItem();

    temp->sideinven = Insideinven::Create();
    temp->sideinven->LoadInven();

    temp->visible = false;
    temp->onoff = false;

    return temp;
}


void GInven::LoadInven()
{

}

void GInven::SaveInven()
{
}

void GInven::Render()
{
    UI::Render();
}

void GInven::Update()
{
    UI::Update();
}


Insideinven* Insideinven::Create(string name)
{
    Insideinven* temp = new Insideinven();
    temp->name = name;
    temp->type = ObType::UI;
    temp->LoadFile("innerinven.xml");
    for (int i = 0; i < temp->numofitem; i++)
    {
        temp->Waeponinven[i] = new GItem();
    }
    for (int i = 0; i < temp->numofitem; i++)
    {
       temp->Helmetinven[i] = new GItem();
    }
    for (int i = 0; i < temp->numofitem; i++)
    {
        temp->Armorinven[i] = new GItem();
    }
    for (int i = 0; i < temp->numofitem; i++)
    {
       temp->Gloveinven[i] = new GItem();
    }
    for (int i = 0; i < temp->numofitem; i++)
    {
        temp->Bootsinven[i] = new GItem();
    }
    for (int i = 0; i < temp->numofitem; i++)
    {
       temp->Useableinven[i] = new GItem();
    }

    return temp;
}

void Insideinven::LoadInven()
{
    ifstream fin;
    
    string slot;
    // ���ϼ����ؼ� �ֱ�
    fin.open("../Contents/loadtest.txt", ios::in);

    int num;
    fin >> num;
    // �ε�&���̺����� ���� ���⿡ �����Ҷ� Load�ϰԵǰ� ���� ���ʽ���� ���������� �����۵���

    for (int i = 0; i < num; i++)
    {
        int type;
        fin >> type;

        int idx;        // �����۸���Ʈ�� ������ΰ�
        fin >> idx;
        if (type == 1)   //����
        {
            Waeponinven[waeponidx]->listidx = idx;
            slot = "Wslot" + to_string(waeponidx);
            Find(slot)->material->diffuseMap
                = RESOURCE->textures.Load(GInven::itemList[idx]->file);
            waeponidx++;
        }
        if (type == 2)   // ����
        {
            Helmetinven[helmetidx]->listidx = idx;
            slot = "Hslot" + to_string(helmetidx);
            Find(slot)->material->diffuseMap
                = RESOURCE->textures.Load(GInven::itemList[idx]->file);
            helmetidx++;
        }
        if (type == 3)   // �Ƹ�
        {
            Armorinven[armoridx]->listidx = idx;
            slot = "Aslot" + to_string(armoridx);
            Find(slot)->material->diffuseMap
                = RESOURCE->textures.Load(GInven::itemList[idx]->file);
            armoridx++;
        }
        if (type == 4)   // �尩
        {
            Gloveinven[gloveidx]->listidx = idx;
            slot = "Gslot" + to_string(gloveidx);
            Find(slot)->material->diffuseMap
                = RESOURCE->textures.Load(GInven::itemList[idx]->file);
            gloveidx++;
        }
        if (type == 5)   // ����
        {
            Bootsinven[bootsidx]->listidx = idx;
            slot = "Bslot" + to_string(bootsidx);
            Find(slot)->material->diffuseMap
                = RESOURCE->textures.Load(GInven::itemList[idx]->file);
            bootsidx++;
        }
        if (type == 6)   //�����
        {
            Useableinven[useableidx]->listidx = idx;
            slot = "Uslot" + to_string(useableidx);
            Find(slot)->material->diffuseMap
                = RESOURCE->textures.Load(GInven::itemList[idx]->file);
            useableidx++;
        }
    }
}

void Insideinven::SaveInven()
{
}

void Insideinven::Render()
{
    UI::Render();
}

void Insideinven::Update()
{
    UI::Update();
}

void Insideinven::Open()
{

}
