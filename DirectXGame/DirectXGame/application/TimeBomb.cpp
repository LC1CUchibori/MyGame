#include "TimeBomb.h"
#include <cassert>
#include <algorithm>
#include <cmath>


TimeBomb::TimeBomb()
{
}

TimeBomb::~TimeBomb()
{
}

void TimeBomb::Initialize(KamataEngine::Model* model,KamataEngine::Model* insideModel, KamataEngine::Camera* camera, const KamataEngine::Vector3& pos)
{
    assert(model);

    model_ = model;
    insideModel_ = insideModel;
    camera_ = camera;

    status_.position_ = pos;

    worldTransform_.Initialize();
    worldTransform_.translation_ = status_.position_;

    insideWorldTransform_.Initialize();
    insideWorldTransform_.translation_ = pos;

}

void TimeBomb::Update()
{
    if (!status_.isAlive_) return;

    status_.timer_++;

    // ===== スケール演出 =====
    const float spikeStartTime = explodeTime_ - 40.0f;

    if (status_.timer_ >= spikeStartTime) {
        float t = (status_.timer_ - spikeStartTime) / 40.0f;
        if (t > 1.0f) t = 1.0f;

        scale_ = 1.0f + t * 0.5f;

        float spikeT = std::pow(t, 0.5f);
        spikeT = std::clamp(spikeT, 0.0f, 1.0f);

        const float minSpikeScale = 0.3f; // 

        float spikeScale = minSpikeScale + spikeT * (1.0f - minSpikeScale);

        insideWorldTransform_.scale_ = {
            spikeScale,
            spikeScale,
            spikeScale
        };


        status_.showSpike_ = true;
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


    // ===== 爆発開始 =====
    if (status_.timer_ == explodeTime_) {
        status_.exploded_ = true;
        status_.explodeTimer_ = 0;
        status_.showSpike_ = true; 
    }

    // ===== 爆発中 =====
    if (status_.exploded_) {
        status_.explodeTimer_++;
        scale_ = 2.0f; // 爆発サイズ

        insideWorldTransform_.scale_ = { 2.5f, 2.5f, 2.5f }; // 爆発サイズ

        if (status_.explodeTimer_ >= 5) {
            status_.exploded_ = false;
            status_.isAlive_ = false;
            status_.showSpike_ = false;
        }
    }
    worldTransform_.translation_ = status_.position_;

    // ===== 揺れ演出 =====
    if (!status_.exploded_) {
        const float shakeSpeed = 0.4f;   // ← 追加
        const float shakePower = 0.15f;  // ← 追加

        float shake =
            std::sin(status_.timer_ * shakeSpeed) * shakePower;

        worldTransform_.translation_.x += shake;
    }

}

void TimeBomb::Draw(KamataEngine::Camera* camera)
{
    if (!status_.isAlive_) return;

    worldTransform_.scale_ = { scale_, scale_, scale_ };

    worldTransform_.UpdateMatrix();
    worldTransform_.TransferMatrix();
    model_->Draw(worldTransform_, *camera);

    // 大きくなり始めたら棘を描画
    if (status_.showSpike_ && insideModel_) {
        insideWorldTransform_.translation_ = worldTransform_.translation_;
        insideWorldTransform_.UpdateMatrix();
        insideWorldTransform_.TransferMatrix();
        insideModel_->Draw(insideWorldTransform_, *camera);
    }
}

void TimeBomb::Kill()
{
    status_.isAlive_ = false;
}
