#pragma once
class Window :public Singleton<Window>
{
	//static
private:
	static LRESULT CALLBACK WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);


	//member
private:
	static Scene* main;
	bool isAxis = true;
public:
	void	Create();
	void	Destroy();
	WPARAM	Run(Scene* main);
	void	Load();
	void	Save();

	// �����Ҷ� ������Ʈ�� ��׷����°� �¿����Ҽ��ֵ���
	void	SetAxis(bool tnf) { isAxis = tnf; }
};

