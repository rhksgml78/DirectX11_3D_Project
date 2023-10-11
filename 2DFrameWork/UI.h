#pragma once
class UI : public Actor
{
public:
	Vector2			 pivot;
	static UI*		 Create(string name = "UI");
	virtual void	 Update() override;
	bool			 Press = false;
	function<void()> mouseOver = nullptr; //���콺�� ���������ҋ�
	function<void()> mouseDown = nullptr; // ������ �������� �ѹ�
	function<void()> mousePress = nullptr; // ������ ������
	function<void()> mouseUp = nullptr; //������ ��������

	bool MouseOver();
	void	RenderDetail();
	void Render();
};
