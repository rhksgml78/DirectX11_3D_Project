#pragma once
#define MAX_BONE 256
class Skeleton
{
    static ID3D11Buffer* bonesBuffer;
public:
    Skeleton() {};
    //                   ���ۿ� ������ ��
    Matrix				 bones[MAX_BONE];
    //                   ���Ϸ� ��������
    Matrix				 bonesOffset[MAX_BONE];// * W
    string               file;
    static void CreateStaticMember();
    static void DeleteStaticMember();
    virtual void Set();
    void LoadFile(string file);
    void SaveFile(string file);
};

