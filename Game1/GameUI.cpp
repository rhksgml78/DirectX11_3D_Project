#include "stdafx.h"

GameUI* GameUI::Create(string name)
{
	// ���ʽ���� �����صѰ͵�
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

	//����ϰ� �������Ҷ� �ؿ� �ڵ� �����ϱ� ������� �߰� 
	//������ �κ����ο� �޼ս����� ����� Equipinit()�ÿ� ������ ��� �ִٸ� �ٷ� �������ٰ�
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
	 
	// �κ��丮 on/off
	{
		/*
			�κ��丮 �⺻ ����
			�����۵����Ϳ� �ִ� �������ε���(��ȣ)�� �������� �۵�
			���� ����ó�� ����Ϸ��� GItem�迭�� �ƴ� int�� �迭���̾�� ��(GItem.h class GInven ����)

			�ǹ��� �ҷ��� �������� loader�� ���� �θ� �� xml������ �����Ͽ�
			itemlist[]->xmlfile�� �����صѰ� �ǹ��� ���� �����۵��� xml�� ����� 

			������ �÷��̾ ������ �޾ƿ� �� �ش� �÷��̾��� ������Ͽ� (ex ���� player->Set����() )
			xml�� ��ü���ִ� �Լ����� �����۸���Ʈ���� ������ xml�� �ҷ��� (GItem.cpp CreateStaticMember() ����)
			�� �������� ���α׷��� ó�� �����Ҷ� �����۵��� xml�� �ѹ��� �θ����� ������ִ°� ����
			�߰��� xml�� ���ʷε�ÿ� ���� �ɸ� �� �ֱ⿡ ���ʷε����� ����� �� �ٷ� �ҷ��� �� �ֵ��� (APlayer.cpp init()����)
		*/


		if (INPUT->KeyDown(VK_ESCAPE))  // esc�� ���â on/off
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

	// ��������� ����(����Ű)
	{
		// ������
		if (INPUT->KeyDown(VK_RIGHT))
		{
			string textname;
			string xml;

			if (inven->rightSlot[0]->listidx == -1) return;
			if (Ridx >= inven->rightSlotSize) return;

			Ridx = (Ridx + 1) % inven->rightSlotSize;
			if (inven->rightSlot[Ridx]->listidx == -1) Ridx = 0;
			//			   ��ü�����  [���õ� ���迭�������� ����Ű�� ��ȣ] ��° ������
			auto item = inven->itemList[inven->rightSlot[Ridx]->listidx];
			if (inven->rightSlot[Ridx]->listidx != -1)
			{
				textname = item->file;
				xml = item->xmlfile;
			}
			else if (EquipPreset->Find("RightSlot")->material->diffuseMap->file
				== "EquipSlot.png" && inven->rightSlot[0])	// 0�� ����ĭ�� �ְ� �⺻�Ͻ�
			{
				textname = inven->itemList[inven->rightSlot[0]->listidx]->file;
			}

			EquipPreset->Find("RightSlot")->material->diffuseMap
				= RESOURCE->textures.Load(textname);
			player->Set_Rhandfile(xml);
		}
		// ����
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
				== "LeftSlot.png" && inven->leftSlot[0])	// 0�� ����ĭ�� �ְ� �⺻�Ͻ�
			{
				textname = inven->itemList[inven->leftSlot[0]->listidx]->file;
			}

			EquipPreset->Find("LeftSlot")->material->diffuseMap
				= RESOURCE->textures.Load(textname);

			player->Set_Lhandfile(xml);
		}
		// �Ʒ�
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
				== "FlaskSlot.png" && inven->useableSlot[0])	// 0�� ����ĭ�� �ְ� �⺻�Ͻ�
			{
				textname = inven->itemList[inven->useableSlot[0]->listidx]->file;
			}

			EquipPreset->Find("FlaskSlot")->material->diffuseMap
				= RESOURCE->textures.Load(textname);
			inven->useableidx = inven->itemList[inven->useableSlot[Uidx]->listidx]->listidx;
		}
		// ��
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
				== "MagicSlot.png" && inven->skillSlot[0])	// 0�� ����ĭ�� �ְ� �⺻�Ͻ�
			{
				textname = inven->itemList[inven->skillSlot[0]->listidx]->file;
			}

			EquipPreset->Find("MagicSlot")->material->diffuseMap
				= RESOURCE->textures.Load(textname);
		}
	}

	// �κ����� ������Ʈ -> �κ��丮�� ����������
	if (inven->onoff == true)
	{
		// �����κ�(�����κ�)
		if (subinven)
		{
			for (const auto& slotName : inven->subslotNames)	// ��������Ž���ؼ�
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

							player->SetArmorfile(inven->itemList[temidx]->xmlfile);	// ���� ����Ű�� �����۹ٲٴ��Ͱ� ����

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

		// �����κ�
		if (subinven == false)
		{
			for (const auto& slotName : inven->slotNames)	// ��������Ž���ؼ�
			{
				if (dynamic_cast<UI*>(inven->Find(slotName))->MouseOver())
				{
					int idx;

					auto it = itemToSlotMap.find(slotName);	// ���� ��

					if (it != itemToSlotMap.end())
					{
						if (it->second->listidx != -1)
						{
							idx = it->second->listidx;	// ���� �̾ƿ���

							// ���������� ���� ���
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
						//��Ŭ��(����)
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
						// ��Ŭ��(����)
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

	// �������ͽ��� (�÷��̾�)
	StatusBar->Find("HpBar")->scale.x = (player->GetHP() / (float)player->GetMaxHP());
	if (player->GetHP() < 0)
	{
		player->HealHP(1);
	}

	// ���ǻ�� �����ʿ�
	if (INPUT->KeyDown('R'))
	{

		//ü�������̸�
		if (inven->useableidx == 0 && hpflasknum > 0)
		{
			hpflasknum--;
			cout << "ü�¿���Ʈ���" << endl;
			cout << "����ü�¿���Ʈ���� : " << hpflasknum << endl;
			player->HealHP(inven->itemList[0]->detail);
			if (hpflasknum <= 0)
			{
				// �������� ���� ��
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
		//��������
		if (inven->useableidx == 11 && mpflasknum > 0)
		{
			mpflasknum--;
			cout << "��������Ʈ���" << endl;
			cout << "������������Ʈ���� : " << mpflasknum << endl;
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
		�̺�Ʈȭ�� ex)youdied
		if������ ��Ȳ�� bool�� ����� bool�� true��
		Event->material->diffuseMap = RESOURCE->textures.Load("����.png");
		�� ���� �̹��� �ٲٰ� �ؿ��� bool���� ���� if�����ؼ� ����
	*/
	if (player->GetHP() <= 0)	//������ died ���� bool������ üũ (���� ����)
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
	//if(������ ���� on/off üũ)
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
	//																		���� �������������� �̽��� ��
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
		// �̸�
		int X = 0.5 * App.GetWidth();

		Long4 NB = { X - 100, (long)(0.1 * App.GetHeight()), X + 200, (long)(0.1 * App.GetHeight() + 400) };

		if(info_name != L"")	// �̸��� ������ üũ -> ������ �ȶ��
		{ 
			DWRITE->RenderText(info_name, RECT{ NB.a, NB.b, NB.c, NB.d}, 50.0f, L"Neo�ձٸ�");

			// ������(��, ����)
			if (info_type >= 1 && info_type <= 5)
			{
				Long4 DB = { X - 100, (long)(0.2 * App.GetHeight()), X + 100, (long)(0.2 * App.GetHeight() + 200) };
				if (info_type == 1)
					DWRITE->RenderText(L"Damage : " + to_wstring(info_detail), RECT{ DB.a, DB.b, DB.c, DB.d }, 30.0f, L"Neo�ձٸ�");
				if (info_type >= 2 && info_type <= 5)
					DWRITE->RenderText(L"Defence : " + to_wstring(info_detail), RECT{ DB.a, DB.b, DB.c, DB.d }, 30.0f, L"Neo�ձٸ�");

				// �䱸�߷�
				Long4 WB = { X - 100, (long)(0.25 * App.GetHeight()), X + 100, (long)(0.25 * App.GetHeight() + 200) };

				DWRITE->RenderText(L"Weight : " + to_wstring(info_weight), RECT{ WB.a, WB.b, WB.c, WB.d}, 30.0f, L"Neo�ձٸ�");
			}
			// ������(ȸ����)
			if (info_type == 6)
			{
				Long4 UB = { X - 100, (long)(0.2 * App.GetHeight()), X + 100, (long)(0.2 * App.GetHeight() + 200) };

				DWRITE->RenderText(L"HP : " + to_wstring(info_detail), RECT{ UB.a, UB.b, UB.c, UB.d }, 30.0f, L"Neo�ձٸ�");
			}
		}
	}
}

void GameUI::Equipinit()
{

	if (inven->sideinven->Waeponinven[0]->listidx != -1)
	{		//���� �������� ����				// �����κ��� 1��° ��
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
		hpflaskmax = inven->itemList[inven->useableSlot[0]->listidx]->nums;	// ü�������� 0������
		inven->useableidx = inven->sideinven->Useableinven[0]->listidx;
	}
	if (inven->sideinven->Useableinven[1]->listidx != -1)
	{
		inven->useableSlot[1]->listidx = inven->sideinven->Useableinven[1]->listidx;
		inven->Find("Useable1")->material->diffuseMap
			= RESOURCE->textures.Load(inven->itemList[inven->useableSlot[1]->listidx]->file);
		mpflaskmax = inven->itemList[inven->useableSlot[1]->listidx]->nums;	// ���������� 1������
	}

}

