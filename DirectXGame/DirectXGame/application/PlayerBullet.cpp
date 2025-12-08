#include "PlayerBullet.h"

using namespace KamataEngine;

PlayerBullet::PlayerBullet() {}
PlayerBullet::~PlayerBullet() {}

void PlayerBullet::Initialize(Model* model, Camera* camera, const Vector3& pos, const Vector3& direction) {
    model_ = model;
    camera_ = camera;
    position_ = pos;
    direction_ = direction;

    worldTransform_.Initialize();
    worldTransform_.translation_ = position_;
    worldTransform_.UpdateMatrix();
}

void PlayerBullet::Update() {
    if (!isActive_) return;

    // 弾にスピード付与
    position_.x +=direction_.x* speed_;
    position_.y +=direction_.y* speed_; 

    // 画面外にでたら削除
    if (position_.x < -200.0f || position_.x > 200.0f ||
        position_.y < -200.0f || position_.y > 200.0f)
    {
        isActive_ = false;
    }

    worldTransform_.translation_ = position_;
    worldTransform_.UpdateMatrix();
}

void PlayerBullet::Draw(Camera* camera) {
    if (isActive_) {
        model_->Draw(worldTransform_, *camera);
    }
}
