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

    if (status_.timer_ >= explodeTime_) {
        status_.exploded_ = true;
        status_.isAlive_ = false;
    }
    worldTransform_.translation_ = status_.position_;
}

void TimeBomb::Draw(KamataEngine::Camera* camera)
{
    if (!status_.isAlive_) return;

    worldTransform_.UpdateMatrix();
    worldTransform_.TransferMatrix();
    model_->Draw(worldTransform_, *camera);
}

void TimeBomb::Kill()
{
    status_.isAlive_ = false;
}
