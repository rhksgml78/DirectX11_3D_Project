#pragma once
class Sound : public Singleton<Sound>
{
    struct SoundNode
    {
        FMOD::Sound* sound;        //����� ���嵥����
        FMOD::Channel* channel;    //������� ä��
        float volume = 1.0f;
    };
    map<string, SoundNode*> SoundList;
    FMOD::System* system;
public:
    Sound();
    ~Sound();
    bool AddSound(string File, string Key, bool loop = false);
    bool DeleteSound(string Key);
    void Play(string Key);
    void Stop(string Key);
    //�Ͻ�����
    void Pause(string Key);
    //�Ͻ����� ����
    void Resume(string Key);
    void SetVolume(string Key, float scale);
    void SetMasterVolume();
    void Update();
};

