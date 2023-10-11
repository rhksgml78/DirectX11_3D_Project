#pragma once
class SceneManager :public Singleton<SceneManager>
{
private:
    map<string, Scene*> scenes;
    Scene* currentScene = nullptr;
    Scene* nextScene = nullptr;
    bool   isChanging = false;
    float  changingTime = 0.0f;
public:
    ~SceneManager();
    //¾À Ãß°¡
    bool    AddScene(string key, Scene* value);
    //¾À »èÁ¦
    bool    DeleteScene(string key);
    //¾À º¯°æ
    Scene* ChangeScene(string key, float changingTime = 0.0f);
    //¾À °¡Á®¿À±â
    Scene* GetScene(string key);
    //ÇöÀç ¾À °¡Á®¿À±â
    Scene* GetCurrentScene();

    void    Release();
    void    Update();
    void    LateUpdate();
    void    PreRender();
    void    Render();
    void    ResizeScreen();

};

