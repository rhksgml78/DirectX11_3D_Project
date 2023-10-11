#pragma once


class Input : public Singleton<Input>
{
    friend class Window;
private:
    unsigned char   keyState[256];
    unsigned char   keyOldState[256];
    unsigned char   keyMap[256];
    Vector3         wheelStatus;
    Vector3         wheelOldStatus;
    Vector3         oldPostion;
    Vector3         currentPostion;

    //�͸� enum
    enum 
    {
        KEY_INPUT_STATUS_DOWN,
        KEY_INPUT_STATUS_PRESS,
        KEY_INPUT_STATUS_NONE,
        KEY_INPUT_STATUS_UP,
    };
public:
    Vector3 position;       //���콺 ��ġ
    Vector3 NDCPosition;
    Vector3 movePosition;   //���콺 �̵�����
    Vector3 wheelMoveValue;
public:
    Input();
    ~Input();

    bool KeyDown(int KeyCode); //�������� ����1ȸ
    bool KeyPress(int KeyCode);//������������
    bool KeyUp(int KeyCode); //�����ٰ� �������� ����1ȸ

    LRESULT InputProc(UINT message, WPARAM wParam, LPARAM lParam);
    void ClearMousePos(Vector3 pos);
    void Update();
};

