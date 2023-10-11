#pragma once
class GameUI
{
public:
	static GameUI* Create(string name = "GUI");
	void Release();


private:

	// �� �������ͽ���
	UI* StatusBar;

	// ����ü�¹�
	UI* BossHPBar;

	// ���� ������
	UI* EquipPreset;	// ����������
	UI* CheckBox;

	UI* Event;		// �̺�Ʈ �߻���(���,Ŭ���� ��)
	float duration;	// �̺�Ʈ�� �ð�
	float time;		// ����ð�üũ��

	class GInven* inven;

	bool subinven = false;
	string subinvenname = "";
	int	temidx = -1;
	string seletedslot = "";

	int Ridx = 0;
	int Lidx = 0;
	int Uidx = 0;
	int Sidx = 0;

	UI* mouse;
	int select;
	Int2 clickIdx;

	//UI* end;		// �ʿ����� �����
	//Camera* camera;	// �����󱼺��� ī�޶�(end�� ���� �����)

	bool  died = false;
	bool  occur = false;	// ��� �� �̺�Ʈ�߻�
	bool  once = false;	// ��� �� �̺�Ʈ�߻�

	int	  hpflasknum;
	int	  mpflasknum;

	int	  hpflaskmax;
	int	  mpflaskmax;

	float hpbarScale;
	float staminaScale;

private:
	GameUI();
	~GameUI();


	void createBuffer();
	void InfoRender();
	void Equipinit();

public:
	void Update(class APlayer* player);
	void BossBarUpdate(class MonsterBoss* boss);
	void Render();
	void RenderHierarchy();
	bool Getonoff() { return inven->onoff; }
	UI* GetBossBar() { return BossHPBar; }

	//Camera* GetUserCam() { return camera; }



private:

	wstring info_name = L"";
	int info_detail = 0;
	int info_weight = 0;
	int info_type = 0;

	struct Long4
	{
		long a;
		long b;
		long c;
		long d;

		Long4() {};

		Long4(long a, long b, long c, long d)
		{
			this->a = a;
			this->b = b;
			this->c = c;
			this->d = d;
		}

		Long4 operator =(Long4 a)
		{
			this->a = a.a;
			this->b = a.b;
			this->c = a.c;
			this->d = a.d;

			return *this;
		}
	};



	std::unordered_map<string, GItem*> itemToSlotMap;

	std::string w2s(const std::wstring& var)
	{
		static std::locale loc("");
		auto& facet = std::use_facet<std::codecvt<wchar_t, char, std::mbstate_t>>(loc);
		return std::wstring_convert<std::remove_reference<decltype(facet)>::type, wchar_t>(&facet).to_bytes(var);
	}

	std::wstring s2w(const std::string& var)
	{
		static std::locale loc("");
		auto& facet = std::use_facet<std::codecvt<wchar_t, char, std::mbstate_t>>(loc);
		return std::wstring_convert<std::remove_reference<decltype(facet)>::type, wchar_t>(&facet).from_bytes(var);
	}

};

