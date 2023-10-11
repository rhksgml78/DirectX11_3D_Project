#pragma once
class ExploedEffect
{
private:
    Pop* effect;

public:

    ExploedEffect();
    ~ExploedEffect();

    void init(string name);
    void Update();
    void Render();
    void RenderHierarchy();

    void SetPosX(float num) { this->effect->SetLocalPosX(num); }
    void SetPosY(float num) { this->effect->SetLocalPosY(num); }
    void SetPosZ(float num) { this->effect->SetLocalPosZ(num); }
    void SetVelocityScalar(float num) {this->effect->velocityScalar = num; }
    void SetDuration(float num) {this->effect->duration = num; }
    void SetGravity(float num) {this->effect->desc.gravity = num ; }
    void SetScalar(float num) {this->effect->velocityScalar = num ; }
    void SetParticleCount(int num) {this->effect->particleCount = num ; }
    void SetSclae(float num) {this->effect->scale = Vector3(num, num, 0) ; }
    void SetImage(string file) {this->effect->material->diffuseMap->LoadFile(file); }

    // ��� ���� �Լ� �߰��ϱ�
    void Play() { this->effect->Play(); }
    void Stop() { this->effect->Stop(); }

    // ��ġ �����ϱ�
    void SetPos(Vector3 pos) { this->effect->SetWorldPos(pos); }
};

