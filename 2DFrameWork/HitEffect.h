#pragma once
class HitEffect
{
private:
    Pop* effect;

public:

    HitEffect();
    ~HitEffect();

    void init(string name);
    void Update();
    void Render();
    void RenderHierarchy();

    void SetPosX(float num) { this->effect->SetLocalPosX(num); }
    void SetPosY(float num) { this->effect->SetLocalPosY(num); }
    void SetPosZ(float num) { this->effect->SetLocalPosZ(num); }
    void SetDuration(float num) { this->effect->duration = num; }
    void SetGravity(float num) { this->effect->desc.gravity = num; }
    void SetScalar(float num) { this->effect->velocityScalar = num; }
    void SetParticleScale(Vector2 num) { this->effect->particleScale = num; }
    void SetParticleCount(int num) { this->effect->particleCount = num; }
    void SetSclae(float num) { this->effect->scale = Vector3(num, num, 0); }
    void SetImage(string file) { this->effect->material->diffuseMap->LoadFile(file); }

    // ��� ���� �Լ� �߰��ϱ�
    void Play() { this->effect->Play(); }
    void Stop() { this->effect->Stop(); }

    // ��ġ �����ϱ�
    void SetPos(Vector3 pos) { this->effect->SetWorldPos(pos); }

    // ��� ������ Ȯ��
    bool GetPlay() { return this->effect->GetPlay(); }
};

