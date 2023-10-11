#include "stdafx.h"

GameUI* GameUI::Create(string name)
{
	// 최초실행시 생성해둘것들
	GameUI* gui = new GameUI();

	gui->StatusBar = UI::Create("StatusBar");
	gui->StatusBar->LoadFile("StatusBar.xml");
	gui->BossHPBar = UI::Create("BossHPBar");
	gui->BossHPBar->LoadFile("BossHPBar.xml");

	gui->inven = GInven::Create();
	gui->createBuffer();

	gui->EquipPreset = UI::Create("EquipPreset");
	gui->EquipPreset->LoadFile("EquipPreset.xml");

	gui->Equipinit();

	//긴급하게 마무리할때 밑에 코드 수정하기 어려워서 추가 
	//원래는 인벤내부에 왼손슬롯을 만들고 Equipinit()시에 장착할 장비가 있다면 바로 장착해줄것
	gui->EquipPreset->Find("LeftSlot")->material->diffuseMap
		= RESOURCE->textures.Load("Largeshield.png");
	gui->inven->Find("Left0")->material->diffuseMap
		= RESOURCE->textures.Load("Largeshield.png");
	gui->inven->leftSlot[0]->listidx = 13;

	gui->Event = UI::Create("Event");
	gui->Event->LoadFile("Event.xml");
	gui->Event->material->opacity = 0.0f;

	gui->CheckBox = UI::Create("CheckBox");
	gui->CheckBox->LoadFile("CheckBox.xml");

	gui->hpflasknum = gui->hpflaskmax;
	gui->mpflasknum = gui->mpflaskmax;

	return gui;
}

void GameUI::Release()
{

	CheckBox->Release();
	Event->Release();
	EquipPreset->Release();
	inven->Release();
	BossHPBar->Release();
	StatusBar->Release();

	delete this;
}

GameUI::GameUI()
{
}

GameUI::~GameUI()
{
}

void GameUI::Update(class APlayer* player)
{
	 
	// 인벤토리 on/off
	{
		/*
			인벤토리 기본 개념
			아이템데이터에 있는 아이템인덱스(번호)를 기준으로 작동
			따라서 지금처럼 사용하려면 GItem배열이 아닌 int형 배열들이었어도 됨(GItem.h class GInven 참조)

			실물을 불러올 아이템은 loader를 통해 부른 뒤 xml파일을 저장하여
			itemlist[]->xmlfile에 저장해둘것 실물이 없는 아이템들은 xml이 없어도됨 

			적용할 플레이어를 변수로 받아온 뒤 해당 플레이어의 무기소켓에 (ex 현재 player->Set부위() )
			xml을 교체해주는 함수만들어서 아이템리스트에서 저장한 xml을 불러옴 (GItem.cpp CreateStaticMember() 참조)
			이 과정에서 프로그램을 처음 실행할때 아이템들의 xml을 한번씩 부르고나서 덮어씌워주는게 좋다
			중간에 xml을 최초로드시에 렉이 걸릴 수 있기에 최초로딩에서 사용한 뒤 바로 불러올 수 있도록 (APlayer.cpp init()참조)
		*/


		if (INPUT->KeyDown(VK_ESCAPE))  // esc로 장비창 on/off
		{
			if (subinven == false)
			{
				if (inven->onoff == false) inven->onoff = true;
				else inven->onoff = false;
			}
			else if (subinven == true)
			{
				inven->sideinven->Find(subinvenname)->visible = false;
				subinven = false;
			}
		}
		if (inven->onoff == true)
		{
			inven->visible = true;
			EquipPreset->visible = false;
			StatusBar->visible = false;
			if (inven->material->opacity <= 1.0f) inven->material->opacity += DELTA * 3.0f;
		}
		else
		{
			inven->visible = false;
			EquipPreset->visible = true;
			StatusBar->visible = true;
			inven->material->opacity = 0.0f;
		}
	}

	// 장비프리셋 관련(방향키)
	{
		// 오른쪽
		if (INPUT->KeyDown(VK_RIGHT))
		{
			string textname;
			string xml;

			if (inven->rightSlot[0]->listidx == -1) return;
			if (Ridx >= inven->rightSlotSize) return;

			Ridx = (Ridx + 1) % inven->rightSlotSize;
			if (inven->rightSlot[Ridx]->listidx == -1) Ridx = 0;
			//			   전체목록중  [선택된 장비배열아이템이 가르키는 번호] 번째 아이템
			auto item = inven->itemList[inven->rightSlot[Ridx]->listidx];
			if (inven->rightSlot[Ridx]->listidx != -1)
			{
				textname = item->file;
				xml = item->xmlfile;
			}
			else if (EquipPreset->Find("RightSlot")->material->diffuseMap->file
				== "EquipSlot.png" && inven->rightSlot[0])	// 0번 장착칸이 있고 기본일시
			{
				textname = inven->itemList[inven->rightSlot[0]->listidx]->file;
			}

			EquipPreset->Find("RightSlot")->material->diffuseMap
				= RESOURCE->textures.Load(textname);
			player->Set_Rhandfile(xml);
		}
		// 왼쪽
		if (INPUT->KeyDown(VK_LEFT))
		{
			string textname;
			string xml;

			if (inven->leftSlot[0]->listidx == -1) return;
			if (Lidx >= inven->leftSlotSize) return;

			Lidx = (Lidx + 1) % inven->leftSlotSize;
			if (inven->leftSlot[Lidx]->listidx == -1) Lidx = 0;

			auto item = inven->itemList[inven->leftSlot[Lidx]->listidx];
			if (inven->leftSlot[Lidx]->listidx != -1)
			{
				textname = item->file;
				xml = item->xmlfile;
			}
			else if (EquipPreset->Find("LeftSlot")->material->diffuseMap->file
				== "LeftSlot.png" && inven->leftSlot[0])	// 0번 장착칸이 있고 기본일시
			{
				textname = inven->itemList[inven->leftSlot[0]->listidx]->file;
			}

			EquipPreset->Find("LeftSlot")->material->diffuseMap
				= RESOURCE->textures.Load(textname);

			player->Set_Lhandfile(xml);
		}
		// 아래
		if (INPUT->KeyDown(VK_DOWN))
		{
			string textname;

			if (inven->useableSlot[0]->listidx == -1) return;
			if (Uidx >= inven->useableSlotSize) return;

			Uidx = (Uidx + 1) % inven->useableSlotSize;
			if (inven->useableSlot[Uidx]->listidx == -1) Uidx = 0;

			auto item = inven->itemList[inven->useableSlot[Uidx]->listidx];
			if (inven->useableSlot[Uidx]->listidx != -1)
			{
				textname = item->file;
			}
			else if (EquipPreset->Find("FlaskSlot")->material->diffuseMap->file
				== "FlaskSlot.png" && inven->useableSlot[0])	// 0번 장착칸이 있고 기본일시
			{
				textname = inven->itemList[inven->useableSlot[0]->listidx]->file;
			}

			EquipPreset->Find("FlaskSlot")->material->diffuseMap
				= RESOURCE->textures.Load(textname);
			inven->useableidx = inven->itemList[inven->useableSlot[Uidx]->listidx]->listidx;
		}
		// 위
		if (INPUT->KeyDown(VK_UP))
		{
			string textname;

			if (inven->skillSlot[0]->listidx == -1) return;
			if (Sidx >= inven->skillSlotSize) return;

			Sidx = (Sidx + 1) % inven->skillSlotSize;
			if (inven->skillSlot[Sidx]->listidx == -1) Sidx = 0;

			auto item = inven->itemList[inven->skillSlot[Sidx]->listidx];
			if (inven->skillSlot[Sidx]->listidx != -1)
			{
				textname = item->file;
			}
			else if (EquipPreset->Find("MagicSlot")->material->diffuseMap->file
				== "MagicSlot.png" && inven->skillSlot[0])	// 0번 장착칸이 있고 기본일시
			{
				textname = inven->itemList[inven->skillSlot[0]->listidx]->file;
			}

			EquipPreset->Find("MagicSlot")->material->diffuseMap
				= RESOURCE->textures.Load(textname);
		}
	}

	// 인벤관련 업데이트 -> 인벤토리가 켜있을때만
	if (inven->onoff == true)
	{
		// 서브인벤(내부인벤)
		if (subinven)
		{
			for (const auto& slotName : inven->subslotNames)	// 장착슬롯탐색해서
			{
				if (inven->sideinven->Find("waeponinven")->visible == true && slotName[0] == 'W')
				{
					if (dynamic_cast<UI*>(inven->sideinven->Find(slotName))->MouseOver())
					{
						int num = std::stoi(slotName.substr(slotName.length() - 1, slotName.length()));
						temidx = inven->sideinven->Waeponinven[num]->listidx;
						CheckBox->SetWorldPos(inven->sideinven->Find(slotName)->GetWorldPos());

						if (INPUT->KeyDown(VK_LBUTTON))
						{
							int sel = std::stoi(seletedslot.substr(seletedslot.length() - 1, seletedslot.length()));
							if(seletedslot[0] == 'R')
								inven->rightSlot[sel]->listidx = temidx;
							else if(seletedslot[0] == 'L')
								inven->leftSlot[sel]->listidx = temidx;

							inven->Find(seletedslot)->material->diffuseMap
								= RESOURCE->textures.Load(inven->itemList[temidx]->file);

							inven->sideinven->Find(subinvenname)->visible = false;
							subinven = false;

							return;
						}
					}
				}
				if (inven->sideinven->Find("helmetinven")->visible == true && slotName[0] == 'H')
				{
					if (dynamic_cast<UI*>(inven->sideinven->Find(slotName))->MouseOver())
					{
						int num = std::stoi(slotName.substr(slotName.length() - 1, slotName.length()));
						temidx = inven->sideinven->Helmetinven[num]->listidx;
						CheckBox->SetWorldPos(inven->sideinven->Find(slotName)->GetWorldPos());
						if (INPUT->KeyDown(VK_LBUTTON))
						{
							inven->helmetSlot->listidx = temidx;

							inven->Find(seletedslot)->material->diffuseMap
								= RESOURCE->textures.Load(inven->itemList[temidx]->file);

							inven->sideinven->Find(subinvenname)->visible = false;
							subinven = false;

							return;
						}
					}
				}
				if (inven->sideinven->Find("armorinven")->visible == true && slotName[0] == 'A')
				{
					if (dynamic_cast<UI*>(inven->sideinven->Find(slotName))->MouseOver())
					{
						int num = std::stoi(slotName.substr(slotName.length() - 1, slotName.length()));
						temidx = inven->sideinven->Armorinven[num]->listidx;
						CheckBox->SetWorldPos(inven->sideinven->Find(slotName)->GetWorldPos());
						if (INPUT->KeyDown(VK_LBUTTON))
						{
							inven->armorSlot->listidx = temidx;

							inven->Find(seletedslot)->material->diffuseMap
								= RESOURCE->textures.Load(inven->itemList[temidx]->file);

							player->SetArmorfile(inven->itemList[temidx]->xmlfile);	// 위에 방향키로 아이템바꾸던것과 같음

							inven->sideinven->Find(subinvenname)->visible = false;
							subinven = false;

							return;
						}
					}
				}
				if (inven->sideinven->Find("gloveinven")->visible == true && slotName[0] == 'G')
				{
					if (dynamic_cast<UI*>(inven->sideinven->Find(slotName))->MouseOver())
					{
						int num = std::stoi(slotName.substr(slotName.length() - 1, slotName.length()));
						temidx = inven->sideinven->Gloveinven[num]->listidx;
						CheckBox->SetWorldPos(inven->sideinven->Find(slotName)->GetWorldPos());
						if (INPUT->KeyDown(VK_LBUTTON))
						{
							inven->gloveSlot->listidx = temidx;

							inven->Find(seletedslot)->material->diffuseMap
								= RESOURCE->textures.Load(inven->itemList[temidx]->file);

							inven->sideinven->Find(subinvenname)->visible = false;
							subinven = false;

							return;
						}
					}
				}
				if (inven->sideinven->Find("bootsinven")->visible == true && slotName[0] == 'B')
				{
					if (dynamic_cast<UI*>(inven->sideinven->Find(slotName))->MouseOver())
					{
						int num = std::stoi(slotName.substr(slotName.length() - 1, slotName.length()));
						temidx = inven->sideinven->Bootsinven[num]->listidx;
						CheckBox->SetWorldPos(inven->sideinven->Find(slotName)->GetWorldPos());
						if (INPUT->KeyDown(VK_LBUTTON))
						{
							inven->bootsSlot->listidx = temidx;

							inven->Find(seletedslot)->material->diffuseMap
								= RESOURCE->textures.Load(inven->itemList[temidx]->file);

							inven->sideinven->Find(subinvenname)->visible = false;
							subinven = false;

							return;
						}
					}
				}
				if (inven->sideinven->Find("useableinven")->visible == true && slotName[0] == 'U')
				{
					if (dynamic_cast<UI*>(inven->sideinven->Find(slotName))->MouseOver())
					{
						int num = std::stoi(slotName.substr(slotName.length() - 1, slotName.length()));
						temidx = inven->sideinven->Useableinven[num]->listidx;
						CheckBox->SetWorldPos(inven->sideinven->Find(slotName)->GetWorldPos());
						if (INPUT->KeyDown(VK_LBUTTON))
						{
							int sel = std::stoi(seletedslot.substr(seletedslot.length() - 1, seletedslot.length()));
							inven->useableSlot[sel]->listidx = temidx;

							inven->Find(seletedslot)->material->diffuseMap
								= RESOURCE->textures.Load(inven->itemList[temidx]->file);

							inven->sideinven->Find(subinvenname)->visible = false;
							subinven = false;

							return;
						}
					}
				}

				if (temidx != -1)
				{
					info_name = s2w(inven->itemList[temidx]->name);
					info_weight = inven->itemList[temidx]->weight;
					info_type = inven->itemList[temidx]->type;
					info_detail = inven->itemList[temidx]->detail;
					inven->Find("Image")->material->diffuseMap =
						RESOURCE->textures.Load(inven->itemList[temidx]->file);

				}
				else
				{
					info_name = L"";
					inven->Find("Image")->material->diffuseMap =
						RESOURCE->textures.Load("blank.png");
				}
			}
		}

		// 메인인벤
		if (subinven == false)
		{
			for (const auto& slotName : inven->slotNames)	// 장착슬롯탐색해서
			{
				if (dynamic_cast<UI*>(inven->Find(slotName))->MouseOver())
				{
					int idx;

					auto it = itemToSlotMap.find(slotName);	// 슬롯 템

					if (it != itemToSlotMap.end())
					{
						if (it->second->listidx != -1)
						{
							idx = it->second->listidx;	// 정보 뽑아오기

							// 정보렌더용 정보 담기
							info_name = s2w(inven->itemList[idx]->name);
							info_weight = inven->itemList[idx]->weight;
							info_type = inven->itemList[idx]->type;
							info_detail = inven->itemList[idx]->detail;

							inven->Find("Image")->material->diffuseMap =
								RESOURCE->textures.Load(inven->itemList[idx]->file);
						}
						else
						{
							info_name = L"";
							inven->Find("Image")->material->diffuseMap =
								RESOURCE->textures.Load("blank.png");
						}
					}

					{
						//좌클릭(장착)
						if (INPUT->KeyDown(VK_LBUTTON))
						{
							if (slotName[0] == 'R' || slotName[0] == 'L')
							{
								subinven = true;
								inven->sideinven->Find("waeponinven")->visible = true;
								subinvenname = "waeponinven";
							}
							if (slotName[0] == 'A')
							{
								subinven = true;
								inven->sideinven->Find("armorinven")->visible = true;
								subinvenname = "armorinven";
							}
							if (slotName[0] == 'H')
							{
								subinven = true;
								inven->sideinven->Find("helmetinven")->visible = true;
								subinvenname = "helmetinven";
							}
							if (slotName[0] == 'G')
							{
								subinven = true;
								inven->sideinven->Find("gloveinven")->visible = true;
								subinvenname = "gloveinven";
							}
							if (slotName[0] == 'B')
							{
								subinven = true;
								inven->sideinven->Find("bootsinven")->visible = true;
								subinvenname = "bootsinven";
							}
							if (slotName[0] == 'U')
							{
								subinven = true;
								inven->sideinven->Find("useableinven")->visible = true;
								subinvenname = "useableinven";
							}
							seletedslot = slotName;
						}
						// 우클릭(해제)
						if (INPUT->KeyDown(VK_RBUTTON))
						{
							if (slotName[0] == 'R')
							{
								int num = std::stoi(slotName.substr(slotName.length() - 1, slotName.length()));
								inven->rightSlot[num]->listidx = -1;
							}
							if (slotName[0] == 'L')
							{
								int num = std::stoi(slotName.substr(slotName.length() - 1, slotName.length()));
								inven->leftSlot[num]->listidx = -1;
							}
							if (slotName[0] == 'A')
							{
								inven->armorSlot->listidx = -1;
							}
							if (slotName[0] == 'H')
							{
								inven->helmetSlot->listidx = -1;
							}
							if (slotName[0] == 'G')
							{
								inven->gloveSlot->listidx = -1;
							}
							if (slotName[0] == 'B')
							{
								inven->bootsSlot->listidx = -1;
							}
							if (slotName[0] == 'U')
							{
								int num = std::stoi(slotName.substr(slotName.length() - 1, slotName.length()));
								inven->useableSlot[num]->listidx = -1;
							}
							inven->Find(slotName)->material->diffuseMap
								= RESOURCE->textures.Load("blank.png");
						}
					}
					CheckBox->SetWorldPos(inven->Find(slotName)->GetWorldPos());
				}
			}
		}



	}

	// 스테이터스바 (플레이어)
	StatusBar->Find("HpBar")->scale.x = (player->GetHP() / (float)player->GetMaxHP());
	if (player->GetHP() < 0)
	{
		player->HealHP(1);
	}

	// 포션사용 수정필요
	if (INPUT->KeyDown('R'))
	{

		//체력포션이면
		if (inven->useableidx == 0 && hpflasknum > 0)
		{
			hpflasknum--;
			cout << "체력에스트사용" << endl;
			cout << "남은체력에스트갯수 : " << hpflasknum << endl;
			player->HealHP(inven->itemList[0]->detail);
			if (hpflasknum <= 0)
			{
				// 마지막을 마신 후
				inven->itemList[0]->file = "EmptyFlask.png";
				EquipPreset->Find("FlaskSlot")->material->diffuseMap = RESOURCE->textures.Load("EmptyFlask.png");
			}
			else if (hpflasknum == 1)
			{
				inven->itemList[0]->file = "LastFlask.png";
				EquipPreset->Find("FlaskSlot")->material->diffuseMap = RESOURCE->textures.Load("LastFlask.png");
			}
			else if (hpflasknum < hpflaskmax)
			{
				inven->itemList[0]->file = "Flask.png";
				EquipPreset->Find("FlaskSlot")->material->diffuseMap = RESOURCE->textures.Load("Flask.png");
			}

		}
		//마나포션
		if (inven->useableidx == 11 && mpflasknum > 0)
		{
			mpflasknum--;
			cout << "마나에스트사용" << endl;
			cout << "남은마나에스트갯수 : " << mpflasknum << endl;
			player->HealMP(inven->itemList[11]->detail);
			if (mpflasknum <= 0)
			{
				inven->itemList[0]->file = "EmptyFlask.png";
				EquipPreset->Find("FlaskSlot")->material->diffuseMap = RESOURCE->textures.Load("EmptyFlask.png");
			}
			else if (mpflasknum == 1)
			{
				inven->itemList[0]->file = "LastFlask_MP.png";
				EquipPreset->Find("FlaskSlot")->material->diffuseMap = RESOURCE->textures.Load("LastFlask_MP.png");
			}
			else if (mpflasknum < hpflaskmax)
			{
				inven->itemList[0]->file = "Flask_MP.png";
				EquipPreset->Find("FlaskSlot")->material->diffuseMap = RESOURCE->textures.Load("Flask_MP.png");
			}
		}
	}

	/*
		이벤트화면 ex)youdied
		if문으로 상황별 bool값 만들고 bool값 true시
		Event->material->diffuseMap = RESOURCE->textures.Load("파일.png");
		를 통해 이미지 바꾸고 밑에서 bool값에 따른 if생성해서 실행
	*/
	if (player->GetHP() <= 0)	//조건을 died 같은 bool값으로 체크 (추후 수정)
	{
		Event->material->diffuseMap = RESOURCE->textures.Load("you-died.png");
		occur = true;
	}
	if (occur == true)
	{
		Event->material->opacity += DELTA * 0.5f;
		if (Event->material->opacity >= 2.5f)
		{
			player->HealHP(player->GetMaxHP());
			Event->material->opacity = 0.0f;
			occur = false;
			once = true;
		}
	}


	//mouse->Update();
	StatusBar->Update();
	EquipPreset->Update();
	CheckBox->Update();
	Event->Update();
	BossHPBar->Update();
	inven->Update();
	inven->sideinven->Update();
}

void GameUI::Render()
{
	BLEND->Set(true);
	StatusBar->Render();
	//if(보스와 전투 on/off 체크)
	//BossHPBar->Render();
	EquipPreset->Render();
	Event->Render();
	inven->Render();
	inven->sideinven->Render();
	if(inven->onoff == true) CheckBox->Render();
	InfoRender();

	BLEND->Set(false);

}

void GameUI::RenderHierarchy()
{
	if (ImGui::TreeNode("UI"))
	{
		StatusBar->RenderHierarchy();
		BossHPBar->RenderHierarchy();
		EquipPreset->RenderHierarchy();
		CheckBox->RenderHierarchy();
		Event->RenderHierarchy();
		inven->RenderHierarchy();
		inven->sideinven->RenderHierarchy();
		//mouse->RenderHierarchy();

		ImGui::TreePop();
	}
}

void GameUI::BossBarUpdate(MonsterBoss* boss)
{
	//if (boss->GetHP() <= 0 && once == false)
	if (boss->bState == static_cast<MonsterBoss::Boss_State>(8) && once == false)
	{
		Event->material->diffuseMap = RESOURCE->textures.Load("win.png");
		occur = true;
	}
	//																		최초 스케일조정에서 미스난 값
	BossHPBar->Find("HPbar")->scale.x = (boss->GetHP() / (float)boss->GetMaxHP()) * 1.1f;

	BossHPBar->Update();
}

void GameUI::createBuffer()
{
	itemToSlotMap =
	{
		{inven->slotNames[0], inven->rightSlot[0]},
		{inven->slotNames[1], inven->rightSlot[1]},
		{inven->slotNames[2], inven->rightSlot[2]},
		{inven->slotNames[3], inven->leftSlot[0]},
		{inven->slotNames[4], inven->leftSlot[1]},
		{inven->slotNames[5], inven->leftSlot[2]},
		{inven->slotNames[6], inven->armorSlot},
		{inven->slotNames[7], inven->helmetSlot},
		{inven->slotNames[8], inven->gloveSlot},
		{inven->slotNames[9], inven->bootsSlot},
		{inven->slotNames[10], inven->useableSlot[0]},
		{inven->slotNames[11], inven->useableSlot[1]},
		{inven->slotNames[12], inven->useableSlot[2]},
		{inven->slotNames[13], inven->useableSlot[3]},
		{inven->slotNames[14], inven->useableSlot[4]}
	};
}

void GameUI::InfoRender()
{
	if (inven->visible == true)
	{
		// 이름
		int X = 0.5 * App.GetWidth();

		Long4 NB = { X - 100, (long)(0.1 * App.GetHeight()), X + 200, (long)(0.1 * App.GetHeight() + 400) };

		if(info_name != L"")	// 이름이 없는지 체크 -> 없으면 안띄움
		{ 
			DWRITE->RenderText(info_name, RECT{ NB.a, NB.b, NB.c, NB.d}, 50.0f, L"Neo둥근모");

			// 디테일(힘, 방어력)
			if (info_type >= 1 && info_type <= 5)
			{
				Long4 DB = { X - 100, (long)(0.2 * App.GetHeight()), X + 100, (long)(0.2 * App.GetHeight() + 200) };
				if (info_type == 1)
					DWRITE->RenderText(L"Damage : " + to_wstring(info_detail), RECT{ DB.a, DB.b, DB.c, DB.d }, 30.0f, L"Neo둥근모");
				if (info_type >= 2 && info_type <= 5)
					DWRITE->RenderText(L"Defence : " + to_wstring(info_detail), RECT{ DB.a, DB.b, DB.c, DB.d }, 30.0f, L"Neo둥근모");

				// 요구중량
				Long4 WB = { X - 100, (long)(0.25 * App.GetHeight()), X + 100, (long)(0.25 * App.GetHeight() + 200) };

				DWRITE->RenderText(L"Weight : " + to_wstring(info_weight), RECT{ WB.a, WB.b, WB.c, WB.d}, 30.0f, L"Neo둥근모");
			}
			// 디테일(회복량)
			if (info_type == 6)
			{
				Long4 UB = { X - 100, (long)(0.2 * App.GetHeight()), X + 100, (long)(0.2 * App.GetHeight() + 200) };

				DWRITE->RenderText(L"HP : " + to_wstring(info_detail), RECT{ UB.a, UB.b, UB.c, UB.d }, 30.0f, L"Neo둥근모");
			}
		}
	}
}

void GameUI::Equipinit()
{

	if (inven->sideinven->Waeponinven[0]->listidx != -1)
	{		//현재 장착중인 슬롯				// 내부인벤의 1번째 템
		inven->rightSlot[0]->listidx = inven->sideinven->Waeponinven[0]->listidx;
		inven->Find("Right0")->material->diffuseMap
		                = RESOURCE->textures.Load(inven->itemList[inven->rightSlot[0]->listidx]->file);
		EquipPreset->Find("RightSlot")->material->diffuseMap
			= RESOURCE->textures.Load(inven->itemList[inven->rightSlot[0]->listidx]->file);
	}

	if (inven->sideinven->Helmetinven[0]->listidx != -1)
	{		
		inven->helmetSlot->listidx = inven->sideinven->Helmetinven[0]->listidx;
		inven->Find("Helmet")->material->diffuseMap
			= RESOURCE->textures.Load(inven->itemList[inven->helmetSlot->listidx]->file);
	}
	if (inven->sideinven->Armorinven[0]->listidx != -1)
	{
		inven->armorSlot->listidx = inven->sideinven->Armorinven[0]->listidx;
		inven->Find("Armor")->material->diffuseMap
			= RESOURCE->textures.Load(inven->itemList[inven->armorSlot->listidx]->file);
	}
	if (inven->sideinven->Gloveinven[0]->listidx != -1)
	{
		inven->gloveSlot->listidx = inven->sideinven->Gloveinven[0]->listidx;
		inven->Find("Glove")->material->diffuseMap
			= RESOURCE->textures.Load(inven->itemList[inven->gloveSlot->listidx]->file);
	}
	if (inven->sideinven->Bootsinven[0]->listidx != -1)
	{
		inven->bootsSlot->listidx = inven->sideinven->Bootsinven[0]->listidx;
		inven->Find("Boots")->material->diffuseMap
			= RESOURCE->textures.Load(inven->itemList[inven->bootsSlot->listidx]->file);
	}
	if (inven->sideinven->Useableinven[0]->listidx != -1)
	{
		inven->useableSlot[0]->listidx = inven->sideinven->Useableinven[0]->listidx;
		inven->Find("Useable0")->material->diffuseMap
			= RESOURCE->textures.Load(inven->itemList[inven->useableSlot[0]->listidx]->file);
		EquipPreset->Find("FlaskSlot")->material->diffuseMap
			= RESOURCE->textures.Load(inven->itemList[inven->useableSlot[0]->listidx]->file);
		hpflaskmax = inven->itemList[inven->useableSlot[0]->listidx]->nums;	// 체력포션이 0번슬롯
		inven->useableidx = inven->sideinven->Useableinven[0]->listidx;
	}
	if (inven->sideinven->Useableinven[1]->listidx != -1)
	{
		inven->useableSlot[1]->listidx = inven->sideinven->Useableinven[1]->listidx;
		inven->Find("Useable1")->material->diffuseMap
			= RESOURCE->textures.Load(inven->itemList[inven->useableSlot[1]->listidx]->file);
		mpflaskmax = inven->itemList[inven->useableSlot[1]->listidx]->nums;	// 마나포션이 1번슬롯
	}

}

