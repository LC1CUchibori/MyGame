#pragma once
#include <KamataEngine.h>

class PlayerBullet {
public:
    PlayerBullet();
    ~PlayerBullet();

    void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& pos, const KamataEngine::Vector3& direction);
    void Update();
    void Draw(KamataEngine::Camera* camera);

    bool IsActive() const { return isActive_; }
    void SetActive(bool active) { isActive_ = active; }
    KamataEngine::Vector3 GetPosition() const { return position_; }

private:
    // ワールドトランスフォーム
    KamataEngine::WorldTransform worldTransform_;
    // モデル
    KamataEngine::Model* model_ = nullptr;
    // カメラ
    KamataEngine::Camera* camera_ = nullptr;
    // ポジション 
    KamataEngine::Vector3 position_{};

    float speed_ = 0.5f;  // スピード　
    bool isActive_ = true; // 発射フラグ

    KamataEngine::Vector3 direction_; // 弾の進行方向
};
