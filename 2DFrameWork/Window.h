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

	// 렌더할때 오브젝트의 축그려지는것 온오프할수있도록
	void	SetAxis(bool tnf) { isAxis = tnf; }
};

