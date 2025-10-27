#pragma once
#include <KamataEngine.h>

class EnemyBullet {
public:
    EnemyBullet();
    ~EnemyBullet();

    void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& pos);
    void Update();
    void Draw(KamataEngine::Camera* camera);

    bool IsActive() const { return isActive_; }
    void SetActive(bool active) { isActive_ = active; }

    const KamataEngine::Vector3& GetPosition() const { return position_; }
    void SetInactive() { isActive_ = false; }


private:
    KamataEngine::WorldTransform worldTransform_;
    KamataEngine::Model* model_ = nullptr;
    KamataEngine::Camera* camera_ = nullptr;

    KamataEngine::Vector3 position_;
    KamataEngine::Vector3 velocity_;
    bool isActive_ = true;
};
