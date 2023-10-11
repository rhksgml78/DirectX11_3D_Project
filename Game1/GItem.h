#pragma once

class GItem
{
    friend class GInven;
public:
    string  name = "";      //이름
    string  file;           //연결될 텍스처
    string  xmlfile;        //연결될 xml
    int     type;           //타입
    int     listidx = -1;   //아이템리스트에서의 인덱스번호

    int     weight = 0;     //무게
    int     nums;           //갯수(ui쪽에서 사용할때 제대로 안받아져서 변수명 살짝 변경 num변수가 너무 많아서 꼬인듯)
    int     detail;         //공,방어력 등(밑에 세부클래스에서 사용해야하는데 급해서 일단 디테일로 대체)
};

class GWeapon : public GItem
{
public:
    friend class GInven;
    int att;        //공격력

};

class Helmet : public GItem
{
public:
    friend class GInven;
    int df;        //방어력

};

class Armor : public GItem
{
public:
    friend class GInven;
    int df;        //방어력

};

class Glove : public GItem
{
public:
    friend class GInven;
    int df;        //방어력

};

class Boots : public GItem
{
public:
    friend class GInven;
    int df;        //방어력

};

class Useable : public GItem
{
public:
    friend class GInven;
    int num;        //갯수

};

//그려줄 객체 , 내부에 아이템 데이터를 관리
class GInven : public UI
{

public:
    //txt 에서 읽어온 아이템 리스트
    static GItem** itemList;
    static int      itemListSize;

    // 슬롯에서도 아이템의 정보들을 전부 저장하려면 이 형태로 써야하지만 굳이 그렇지않고 
    // 슬롯에 번호만 저장시킨 뒤 해당 번호를 가지고
    // 아이템 리스트에서 번호에 해당하는 아이템을 불러서 사용하는 방식으로 사용중 (코드 간결화)
    // -> int형 배열로 사용하는게 깔끔함 현재는 rightSlot[0]->listidx 혹은 armorSlot->listidx 로 사용하지만
    // int배열이었다면 바로 rightSlot[0]나 armorSlot 만으로 해당 아이템 번호 불러오기 가능
    // (깨닫는 시간이 오래걸려서 귀찮아서 못바꿨음)

    // 부위별로 현재 장착중인 아이템정보 확인할 수 있는 변수도 하나 있으면 좋을듯
    
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
    int    useableidx = -1; // 사용할 사용아이템의 인덱스 번호를 저장할 변수
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