#pragma once
#include <KamataEngine.h>
#include <vector>
#include "PlayerBullet.h"

class Player {
public:
    Player();
    ~Player();

    void Initialize(KamataEngine::Model* model, uint32_t textureHandle, KamataEngine::Camera* camera);
    void Update();
    void Draw(KamataEngine::Camera* camera, uint32_t textureHandle);
    void SetYaw(float yaw);

    void Fire();
    const std::vector<PlayerBullet*>& GetBullets() const { return bullets_; }

    // 死亡処理
    void Kill() { isDead_ = true; }
    bool IsDead() const { return isDead_; }

    // 座標取得
    const KamataEngine::Vector3& GetPosition() const { return worldTransform_.translation_; }

private:
    KamataEngine::Model* model_ = nullptr;
    KamataEngine::Camera* camera_ = nullptr;
    uint32_t textureHandle_ = 0;

    KamataEngine::Vector3 position_{};
    KamataEngine::WorldTransform worldTransform_;
    KamataEngine::Input* input = nullptr;

    std::vector<PlayerBullet*> bullets_;
    KamataEngine::Model* bulletModel_ = nullptr;

    float speed_ = 0.5f;
    float yaw_ = 0.0f;

    bool isDead_ = false;  // 死亡フラグ
};
