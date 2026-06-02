#pragma once
#include <KamataEngine.h>
#include "Model2.h"
#include <vector>

class TitleModel
{
public:
    void Initialize(KamataEngine::Model* model);

    void Update();

    void Draw(KamataEngine::Camera* camera);

    KamataEngine::Vector3 GetPosition() const { return worldTransform_.translation_; }
private:
    KamataEngine::Model* TitleModel_ = nullptr;
    KamataEngine::WorldTransform worldTransform_;

    KamataEngine::Vector3 titlePosition_ = { 0.0f,0.0f,0.0f };
	KamataEngine::Vector3 titleVelocity_ = { 0.5f,0.0f,0.0f };
    KamataEngine::Vector3 rotationSpeed_ = { 0.0f,0.0f,0.0f };
    float leftLimit  = -15.0f;
    float rightLimit = 15.0f;
};

