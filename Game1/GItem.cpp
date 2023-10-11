#include "stdafx.h"
#include "GameUI.h"

// static 변수이기때문에 전역범위에서 초기화를 한번 해야한다.
GItem** GInven::itemList = nullptr;
int   GInven::itemListSize = 0;

void GInven::CreateStaticMember()
{
    ifstream fin;

    // 파일수정해서 넣기
    
    // 현재는 무기와 갑옷만 xml을 저장하고 있는데 assets을 못구해서
    // loader를 통해 xml파일을 만들지 못했기 때문
    // 실물이 있는 아이템을 추가할땐 꼭 assets을 구하고 xml저장까지 확인한 뒤 추가하자
    // 그렇게 완전히 할 수 있다면 밑에 xmlfile 저장하는 주석을 풀고 파일수정하며 사용하면 된다.
    
    fin.open("../Contents/test.db", ios::in);

    // 메모장구성
    // 아이템갯수(사이즈)
    // 타입 번호 별명 이미지이름 스텟 xml파일명
    // 타입 번호 별명 이미지이름 스텟 xml파일명
    // 타입 번호 별명 이미지이름 스텟 xml파일명
    // ...
    // (test.db 연결프로그램 메모장으로 열어서 참조, 추가,삭제에 따라 편집)

    fin >> itemListSize;

    itemList = new GItem * [itemListSize];

    for (int i = 0; i < itemListSize; i++)
    {
        int idx;
        int type;
        fin >> idx; //아이템 번호
        fin >> type; //아이템 타입
        if (type == 0)
        {      
            itemList[idx] = new GItem();        // 기타 템 (현재존재x)
            fin >> itemList[idx]->name;
            fin >> itemList[idx]->file;

        }
        else  if (type == 1)
        {
            GWeapon* weapon = new GWeapon();    // 무기
            fin >> weapon->name;
            fin >> weapon->file;
            fin >> weapon->detail;
            fin >> weapon->xmlfile;
            itemList[idx] = weapon;
        }
        else  if (type == 2)
        {
            Helmet* helmet = new Helmet();    // 뚝배기
            fin >> helmet->name;
            fin >> helmet->file;
            fin >> helmet->detail;
            //fin >> helmet->xmlfile;
            itemList[idx] = helmet;
        }
        else  if (type == 3)
        {
            Armor* armor = new Armor();       // 갑빠
            fin >> armor->name;
            fin >> armor->file;
            fin >> armor->detail;
            fin >> armor->xmlfile;
            itemList[idx] = armor;
        }
        else  if (type == 4)
        {
            Glove* glove = new Glove();       // 장갑
            fin >> glove->name;
            fin >> glove->file;
            fin >> glove->detail;
            //fin >> glove->xmlfile;
            itemList[idx] = glove;
        }
        else  if (type == 5)
        {
            Boots* boots = new Boots();       // 부츠
            fin >> boots->name;
            fin >> boots->file;
            fin >> boots->detail;
            //fin >> boots->xmlfile;
            itemList[idx] = boots;
        }
        else  if (type == 6)
        {
            Useable* useable = new Useable();       // 사용템
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
        cout << i << "번째 아이템의 이름 : " << itemList[i]->name << endl;
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
    // 파일수정해서 넣기
    fin.open("../Contents/loadtest.txt", ios::in);

    int num;
    fin >> num;
    // 로드&세이브기능이 아직 없기에 시작할때 Load하게되고 따라서 최초실행시 가지고있을 아이템들임

    for (int i = 0; i < num; i++)
    {
        int type;
        fin >> type;

        int idx;        // 아이템리스트의 몇번템인가
        fin >> idx;
        if (type == 1)   //무기
        {
            Waeponinven[waeponidx]->listidx = idx;
            slot = "Wslot" + to_string(waeponidx);
            Find(slot)->material->diffuseMap
                = RESOURCE->textures.Load(GInven::itemList[idx]->file);
            waeponidx++;
        }
        if (type == 2)   // 투구
        {
            Helmetinven[helmetidx]->listidx = idx;
            slot = "Hslot" + to_string(helmetidx);
            Find(slot)->material->diffuseMap
                = RESOURCE->textures.Load(GInven::itemList[idx]->file);
            helmetidx++;
        }
        if (type == 3)   // 아머
        {
            Armorinven[armoridx]->listidx = idx;
            slot = "Aslot" + to_string(armoridx);
            Find(slot)->material->diffuseMap
                = RESOURCE->textures.Load(GInven::itemList[idx]->file);
            armoridx++;
        }
        if (type == 4)   // 장갑
        {
            Gloveinven[gloveidx]->listidx = idx;
            slot = "Gslot" + to_string(gloveidx);
            Find(slot)->material->diffuseMap
                = RESOURCE->textures.Load(GInven::itemList[idx]->file);
            gloveidx++;
        }
        if (type == 5)   // 부츠
        {
            Bootsinven[bootsidx]->listidx = idx;
            slot = "Bslot" + to_string(bootsidx);
            Find(slot)->material->diffuseMap
                = RESOURCE->textures.Load(GInven::itemList[idx]->file);
            bootsidx++;
        }
        if (type == 6)   //사용템
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
