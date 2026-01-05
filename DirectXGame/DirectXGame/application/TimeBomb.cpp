#include "TimeBomb.h"

TimeBomb::TimeBomb()
{
}

TimeBomb::~TimeBomb()
{
}

void TimeBomb::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& pos)
{
    assert(model);

    model_ = model;
    camera_ = camera;

    position_ = pos;

    worldTransform_.Initialize();
    worldTransform_.translation_ = position_;
}

void TimeBomb::Update()
{
    if (!status_.isAlive_) return;

    status_.timer_++;

    // ===== スケール演出 =====
    if (status_.timer_ >= 255 && status_.timer_ < 285) {
        // 膨らむ
        float t = (status_.timer_ - 255) / 30.0f;
        scale_ = 1.0f + t * 0.5f;
    }
    else if (status_.timer_ >= 285 && status_.timer_ < 300) {
        // 縮む
        float t = (status_.timer_ - 285) / 15.0f;
        t = t * t;
        scale_ = 1.5f - t * 0.7f;
    }
    else {
        scale_ = 1.0f;
    }


    if (status_.timer_ >= explodeTime_) {
        status_.exploded_ = true;
        status_.isAlive_ = false;
    }
    worldTransform_.translation_ = position_;
}

void TimeBomb::Draw(KamataEngine::Camera* camera)
{
    if (!status_.isAlive_) return;

    worldTransform_.scale_ = { scale_, scale_, scale_ };

    worldTransform_.UpdateMatrix();
    worldTransform_.TransferMatrix();
    model_->Draw(worldTransform_, *camera);
}

void TimeBomb::Kill()
{
    status_.isAlive_ = false;
}
