#include "framework.h"

Input::Input()
{
	//�����ּҷκ��� ������ ���� 0���� �ʱ�ȭ
	ZeroMemory(keyState, sizeof(keyState));
	ZeroMemory(keyOldState, sizeof(keyOldState));
	ZeroMemory(keyMap, sizeof(keyMap));


    position = Vector3(0, 0, 0);
    NDCPosition = Vector3(0, 0, 0);
    wheelStatus = Vector3(0.0f, 0.0f, 0.0f);
    wheelOldStatus = Vector3(0.0f, 0.0f, 0.0f);
    wheelMoveValue = Vector3(0.0f, 0.0f, 0.0f);
    DWORD tLine = 0;
    SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &tLine, 0);
}

Input::~Input()
{
}

bool Input::KeyDown(int KeyCode)
{
    return keyMap[KeyCode] == KEY_INPUT_STATUS_DOWN;
}

bool Input::KeyPress(int KeyCode)
{
    return keyMap[KeyCode] <= KEY_INPUT_STATUS_PRESS;
}

bool Input::KeyUp(int KeyCode)
{
    return keyMap[KeyCode] == KEY_INPUT_STATUS_UP;
}

LRESULT Input::InputProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_LBUTTONDOWN || message == WM_MOUSEMOVE)
    {
        position.x = (float)LOWORD(lParam);
        position.y = (float)HIWORD(lParam);
    }

    if (message == WM_MOUSEWHEEL)
    {
        short tWheelValue = (short)HIWORD(wParam);

        wheelOldStatus.z = wheelStatus.z;
        wheelStatus.z += (float)tWheelValue;
    }

    return TRUE;
}

void Input::ClearMousePos(Vector3 pos)
{
    oldPostion = pos;
    currentPostion = pos;
    movePosition = Vector3();
}

void Input::Update()
{



    //�޸𸮺��� keyOldState = keyState 
    memcpy(keyOldState, keyState, sizeof(keyOldState));

    //������ �迭�� �ʱ�ȭ
    ZeroMemory(keyState, sizeof(keyState));



    ZeroMemory(keyMap, sizeof(keyMap));

    //BOOL a;
    //���� ȣ����¿����� Ű�Է°��� �������� �Լ�
    if (GetKeyboardState(keyState))
    {
        for (UINT i = 0; i < 256; i++)
        {
            byte key = keyState[i] & 0x80;
            //���׿�����   (bool)? 1{} 0{}
            //Ű�� �������¸� 1, �ƴϸ� 0���� ����
            keyState[i] = key ? 1 : 0;

            int oldState = keyOldState[i];
            int state = keyState[i];

            if (oldState == 0 && state == 1)
                keyMap[i] = KEY_INPUT_STATUS_DOWN; //���� 0, ���� 1 - KeyDown
            else if (oldState == 1 && state == 0)
                keyMap[i] = KEY_INPUT_STATUS_UP; //���� 1, ���� 0 - KeyUp
            else if (oldState == 1 && state == 1)
                keyMap[i] = KEY_INPUT_STATUS_PRESS; //���� 1, ���� 1 - KeyPress
            else
                keyMap[i] = KEY_INPUT_STATUS_NONE;
        }
    }

    POINT point;
    GetCursorPos(&point);
    ScreenToClient(App.handle, &point);

    wheelOldStatus.x = wheelStatus.x;
    wheelOldStatus.y = wheelStatus.y;

    wheelStatus.x = float(point.x);
    wheelStatus.y = float(point.y);

    wheelMoveValue = wheelStatus - wheelOldStatus;
    wheelOldStatus.z = wheelStatus.z;

    currentPostion = position;
    movePosition = currentPostion - oldPostion;

    oldPostion = currentPostion;

    //0~ 800  ->   0 ~ 2  -> -1 ~ 1
    NDCPosition.x = position.x / App.GetHalfWidth() - 1.0f;
    //0~ 600 -> 0~ -2 -> 1 ~ -1
    NDCPosition.y = position.y / -App.GetHalfHeight() + 1.0f;
}
