#pragma once
class Sound : public Singleton<Sound>
{
    struct SoundNode
    {
        FMOD::Sound* sound;        //출력할 사운드데이터
        FMOD::Channel* channel;    //출력해줄 채널
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
    //일시정지
    void Pause(string Key);
    //일시정지 해제
    void Resume(string Key);
    void SetVolume(string Key, float scale);
    void SetMasterVolume();
    void Update();
};

