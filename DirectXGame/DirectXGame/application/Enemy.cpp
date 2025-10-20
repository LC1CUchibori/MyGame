#include "Enemy.h"
#include <cassert>

Enemy::Enemy() {
}

Enemy::~Enemy() {
}

void Enemy::Initialize(KamataEngine::Model* model, uint32_t textureHandle, KamataEngine::Camera* camera) {
    assert(model);

    model_ = model;
    textureHandle_ = textureHandle;
    camera_ = camera;

    // 敵の初期位置（奥のほうに配置）
    position_ = { 0.0f, 60.0f, 200.0f }; // ← Zを大きくして奥から出てくる

    worldTransform_.Initialize();
    worldTransform_.translation_ = position_;
    worldTransform_.UpdateMatrix();
    worldTransform_.TransferMatrix();

    // 移動速度設定
    speed_ = 0.5f;       // 左右移動の速度
    approachSpeed_ = 0.8f; // 手前に来る速度
    direction_ = 1.0f;   // 初期は右方向
    isApproaching_ = true; // 最初は接近中
    stopZ_ = -10.0f;     // 手前に来るZ座標
    stopY_ = 10.0f; 
}

void Enemy::Update() {
    if (isApproaching_) {
        // Z方向に手前へ移動
        position_.z -= approachSpeed_;

        // Y座標を線形補間で下降（上→下）
        float t = (200.0f - position_.z) / (200.0f - stopZ_); // 0〜1
        position_.y = 60.0f + t * (stopY_ - 60.0f); // 上から stopY_ まで

        // 停止判定
        if (position_.z <= stopZ_) {
            position_.z = stopZ_;
            position_.y = stopY_;
            isApproaching_ = false;
        }
    }
    else {
        // 左右移動
        position_.x += speed_ * direction_;
        if (position_.x > 20.0f || position_.x < -20.0f) {
            direction_ *= -1;
        }
    }

    // ワールド変換更新
    worldTransform_.translation_ = position_;
    worldTransform_.UpdateMatrix();
    worldTransform_.TransferMatrix();
}



void Enemy::Draw(KamataEngine::Camera* camera, uint32_t textureHandle) {
    model_->Draw(worldTransform_, *camera, textureHandle);
}
