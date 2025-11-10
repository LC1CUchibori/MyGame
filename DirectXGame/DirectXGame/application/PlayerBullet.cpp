#include "PlayerBullet.h"

using namespace KamataEngine;

PlayerBullet::PlayerBullet() {}
PlayerBullet::~PlayerBullet() {}

void PlayerBullet::Initialize(Model* model, Camera* camera, const Vector3& pos) {
    model_ = model;
    camera_ = camera;
    position_ = pos;

    worldTransform_.Initialize();
    worldTransform_.translation_ = position_;
    worldTransform_.UpdateMatrix();
}

void PlayerBullet::Update() {
    if (!isActive_) return;

    position_.y += speed_;  // 前方に進む

    if (position_.z > 250.0f) {
        isActive_ = false;  // 画面外で消える
    }

    worldTransform_.translation_ = position_;
    worldTransform_.UpdateMatrix();
}

void PlayerBullet::Draw(Camera* camera) {
    if (isActive_) {
        model_->Draw(worldTransform_, *camera);
    }
}
