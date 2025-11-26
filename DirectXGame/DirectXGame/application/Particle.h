#pragma once
#include <KamataEngine.h>

class Particle
{
public:
    Particle();
    ~Particle();

    void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera,
        const KamataEngine::Vector3& startPos);

    void Update();
    void Draw();

    bool IsAlive() const { return alive_; }

    void Kill() { alive_ = false; }


private:
    KamataEngine::WorldTransform worldTransform_;
    KamataEngine::Model* model_ = nullptr;
    KamataEngine::Camera* camera_ = nullptr;

    KamataEngine::Vector3 velocity_{};
    float lifetime_ = 0.0f;
    float maxLifetime_ = 0.5f;

    bool alive_ = true;

    float alpha=0.0f;
    KamataEngine::Vector4 color = {};
    KamataEngine::ObjectColor objColor;
};
