#pragma once

class Item
{
    friend class Inven;
    int    type;    //타입
    string name;    //이름
    int    num;     //갯수
    string file;    //연결될 텍스처 or xml

};

class Weapon : public Item
{
public:
    friend class Inven;
    int att;        //공격력

};

//그려줄 객체 , 내부에 아이템 데이터를 관리
class Inven : public UI
{
    //txt 에서 읽어온 아이템 리스트
    static Item**   itemList;
    static int      itemListSize;

    //이중배열로 만들 아이템
    Item*           itemSlot[2][2];
    int             itemSlotSize;
    /*enum class MouseState
    {
        NONE,
        CL,
    };*/
    //마우스 위치에 그려줄 객체
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