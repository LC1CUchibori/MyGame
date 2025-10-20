#pragma once
#include <KamataEngine.h>
#include "Model2.h"
#include <vector>

class Crosshair3D {
public:
    void Initialize(KamataEngine::Model* model);

    void Update();

    void Draw(KamataEngine::Camera* camera);

    KamataEngine::Vector3 GetPosition() const { return worldTransform_.translation_; }

private:
    KamataEngine::Model* model_ = nullptr;
    KamataEngine::WorldTransform worldTransform_;
    float speed_ = 0.5f;
};
