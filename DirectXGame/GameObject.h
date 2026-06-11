#pragma once
#include <KamataEngine.h>
class GameObject
{
public:
    virtual ~GameObject() = default;

    virtual void Initialize(KamataEngine::Model* mode, KamataEngine::Camera* camera) = 0;
    virtual void Update() = 0;
    virtual void Draw(KamataEngine::Camera* camera) = 0;

    virtual bool IsAlive() const {
        return true;
    }

private:
    // モデル
    KamataEngine::Model* model_ = nullptr;
    // カメラ
    KamataEngine::Camera* camera_ = nullptr;

};

