#include "stdafx.h"

// static 변수이기때문에 전역범위에서 초기화를 한번 해야한다.
Item** Inven::itemList = nullptr;
int   Inven::itemListSize = 0;

void Inven::CreateStaticMember()
{
    ifstream fin;

    fin.open("../Contents/aaa111.db", ios::in);

    fin >> itemListSize;

    itemList = new Item * [itemListSize];

    for (int i = 0; i < itemListSize; i++)
    {
        int idx;
        int type;
        fin >> idx; //아이템 번호
        fin >> type; //아이템 타입
        if (type == 0)
        {
            itemList[idx] = new Item();
            fin >> itemList[idx]->name;
            fin >> itemList[idx]->file;

        }
        else  if (type == 1)
        {
            Weapon* weapon = new Weapon();
            fin >> weapon->name;
            fin >> weapon->file;
            fin >> weapon->att;
            itemList[idx] = weapon;
        }
    }
}

void Inven::DeleteStaticMember()
{
    SafeDeleteArray(itemList);
}

Inven* Inven::Create(string name)
{
    Inven* temp = new Inven();
    temp->name = name;
    temp->type = ObType::UI;
    temp->select = 0;

    //temp->mesh = RESOURCE->meshes.Load("6.UI.mesh");
    //temp->shader = RESOURCE->shaders.Load("6.Cube.hlsl");
    return temp;
}

void Inven::LoadInven()
{
    mouse = UI::Create();
    mouse->LoadFile("Ui_Sample.xml");
    mouse->visible = false;

    ifstream fin;

    fin.open("../Contents/aabb.obb", ios::in);

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            itemSlot[i][j] = new Item();
            string temp;
            fin >> temp;
            if (temp == "Y")
            {
                int idx;
                fin >> idx;
                int num;
                fin >> num;
                itemSlot[i][j]->num = num;
                itemSlot[i][j]->name = itemList[idx]->name;
                itemSlot[i][j]->type = itemList[idx]->type;
                string slot = "Slot" + to_string(i) + to_string(j);
                Find(slot.c_str())->material->diffuseMap
                    = RESOURCE->textures.Load(itemList[idx]->file);
                itemSlot[i][j]->file = itemList[idx]->file;
            }
            else
            {
                itemSlot[i][j]->num = 0;
                string slot = "Slot" + to_string(i) + to_string(j);
                Find(slot.c_str())->material->diffuseMap
                    = RESOURCE->textures.Load("slot.png");
                itemSlot[i][j]->file = "slot.png";
            }
        }
    }


}

void Inven::SaveInven()
{
}

void Inven::Render()
{
    UI::Render();
    BLEND->Set(true); // 투명도조절 opacity 때문에 블렌드 세팅해줘야한다.
    mouse->SetWorldPos(INPUT->NDCPosition);
    mouse->Update();
    mouse->Render();
    BLEND->Set(false);
}

void Inven::Update()
{
    if (select == 1)
    {
        if (INPUT->KeyUp(VK_LBUTTON))
        {
            // 마우스 그냥 떼면 스왑하지않고 visible끄고 마우스상태 0으로.
            mouse->visible = false;
            select = 0;

            //스왑
            for (int i = 0; i < 2; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    string slot = "Slot" + to_string(i) + to_string(j);
                    UI* ui = (UI*)Find(slot.c_str());
                    if (ui->MouseOver())
                    {
                        // 클릭된 위치와 이동해서 마우스오버되어있는 좌표의 정보를 스왑 한다.
                        swap(itemSlot[i][j], itemSlot[clickIdx.x][clickIdx.y]);

                        // 정보 스왑외 가지고있는 이미지(디퓨즈맵)도 바꾸어준다.
                        string slot2 = "Slot" + to_string(clickIdx.x) + to_string(clickIdx.y);
                        swap(Find(slot.c_str())->material->diffuseMap,
                            Find(slot2.c_str())->material->diffuseMap);

                        // 클릭했던좌표를 없는 좌표로 바꾸어버린다.
                        clickIdx.x = -1;
                        clickIdx.y = -1;
                    }
                }
            }
        }
    }

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            string slot = "Slot" + to_string(i) + to_string(j);
            UI* ui = (UI*)Find(slot.c_str());

            if (ui->MouseOver())
            {
                if (INPUT->KeyDown(VK_LBUTTON) and not select)
                {
                    select = 1;
                    mouse->material->diffuseMap
                        = RESOURCE->textures.Load(
                            itemSlot[i][j]->file);
                    mouse->material->opacity = 0.3f;
                    mouse->visible = true;
                    clickIdx.x = i;
                    clickIdx.y = j;
                }
            }
        }
    }


    UI::Update(); // 재정의 되고나서 부모의 업데이트가 안 불려지기때문에 따로 업데이트 한다.
}
