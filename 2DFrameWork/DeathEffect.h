#pragma once
class DeathEffect
{
private:
    Rain* effect;

public:

    DeathEffect();
    ~DeathEffect();

    void init(string name);
    void Update();
    void Render();
    void RenderHierarchy();

    void SetDuration(float num) { this->effect->duration = num; }
    void SetVelocity(Vector3 velocity) { this->effect->desc.velocity = velocity; }
    void SetRange(Vector3 range) { this->effect->desc.range = range; }
    void SetParticleScale(Vector2 scale) { this->effect->particleScale = scale; }
    void SetParticleCount(int num) { this->effect->particleCount = num; }
    void SetSclae(float num) { this->effect->scale = Vector3(num, num, 0); }
    void SetImage(string file) { this->effect->material->diffuseMap->LoadFile(file); }

    // 재생 정지 함수 추가하기
    void Play() { this->effect->Play(); }
    void Stop() { this->effect->Stop(); }

    // 위치 세팅하기
    void SetPos(Vector3 pos) { this->effect->SetWorldPos(pos); }
    void SetPosY(float pos) { this->effect->SetWorldPosY(pos); }

    bool GetPlay() { return this->effect->GetPlay(); }
    float GetHalfY() { return this->effect->desc.range.y * 0.5f; }
};

