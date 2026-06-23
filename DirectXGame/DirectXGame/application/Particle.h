#pragma once
#include <KamataEngine.h>

class Particle
{
public:
    Particle();
    ~Particle();

    void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera,const KamataEngine::Vector3& startPos);

    void Update();

    void Draw();

    void Kill() { alive_ = false; }

    bool IsAlive() const { return alive_; }

private:
    // ワールドトランスフォーム
    KamataEngine::WorldTransform worldTransform_;
    // モデル
    KamataEngine::Model* model_ = nullptr;
    // カメラ
    KamataEngine::Camera* camera_ = nullptr;
    // 速度
    KamataEngine::Vector3 velocity_{};

    float lifetime_ = 0.0f;
    float maxLifetime_ = 0.8f;

    bool alive_ = false;

    // 色
    float alpha = 0.0f;
    KamataEngine::Vector4 color = {};
    KamataEngine::ObjectColor objColor;
};
