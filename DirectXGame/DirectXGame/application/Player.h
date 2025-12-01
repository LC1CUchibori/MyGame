#pragma once
#include <KamataEngine.h>
#include <vector>
#include "PlayerBullet.h"
#include "Particle.h"

class Player {
public:
    Player();

    ~Player();

    void Initialize(KamataEngine::Model* model, uint32_t textureHandle, KamataEngine::Camera* camera);

    void Update();

    void Draw(KamataEngine::Camera* camera, uint32_t textureHandle);

    void SetYaw(float yaw);

    void Fire();

    // 死亡処理
    void Kill();

    void SetCanShoot(bool canShoot) { canShoot_ = canShoot; }


    bool CanShoot() const { return canShoot_; }
    bool IsDead() const { return isDead_; }

    // プレイヤー弾Getter
    const std::vector<PlayerBullet*>& GetBullets() const { return bullets_; }
    // 座標取得Getter
    const KamataEngine::Vector3& GetPosition() const { return worldTransform_.translation_; }

private:
    // モデル
    KamataEngine::Model* model_ = nullptr;
    // カメラ
    KamataEngine::Camera* camera_ = nullptr;
    uint32_t textureHandle_ = 0;
    // ポジション
    KamataEngine::Vector3 position_{};
    // ワールドトランスフォーム
    KamataEngine::WorldTransform worldTransform_;
    // インプット
    KamataEngine::Input* input = nullptr;

    // プレイヤー弾
    std::vector<PlayerBullet*> bullets_;
    KamataEngine::Model* bulletModel_ = nullptr;

    // プレイヤーパーティクル
    std::vector<Particle*> particles_;
    KamataEngine::Model* particleModel_ = nullptr;

    // 移動速度
    float speed_ = 0.5f;
    // 回転
    float yaw_ = 0.0f;

    // フラグ類
    bool isDead_ = false;  // 死亡フラグ
    bool canShoot_ = true; // 発射フラグ
};
