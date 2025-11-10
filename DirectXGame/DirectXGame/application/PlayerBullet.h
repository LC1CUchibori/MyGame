#pragma once
#include <KamataEngine.h>

class PlayerBullet {
public:
    PlayerBullet();
    ~PlayerBullet();

    void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& pos);
    void Update();
    void Draw(KamataEngine::Camera* camera);

    bool IsActive() const { return isActive_; }
    void SetActive(bool active) { isActive_ = active; }
    KamataEngine::Vector3 GetPosition() const { return position_; }

private:
    KamataEngine::WorldTransform worldTransform_;
    KamataEngine::Model* model_ = nullptr;
    KamataEngine::Camera* camera_ = nullptr;
    KamataEngine::Vector3 position_{};
    float speed_ = 0.5f;
    bool isActive_ = true;
};
