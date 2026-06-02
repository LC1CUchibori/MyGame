#include "EnemyBullet.h"
#include <cassert>

using namespace KamataEngine;

EnemyBullet::EnemyBullet() {}
EnemyBullet::~EnemyBullet() {}

void EnemyBullet::Initialize(Model* model, Camera* camera, const Vector3& pos) {
    assert(model);
    model_ = model;
    camera_ = camera;
    position_ = pos;

    // ======= 下方向（Yマイナス）に発射 =======
    velocity_ = { 0.0f, -1.0f, 0.0f };

    worldTransform_.Initialize();
    worldTransform_.translation_ = position_;
}

void EnemyBullet::Update() {
    if (!isActive_) return;

    // 下に移動
    position_.y += velocity_.y * 0.3f; // 弾速

    // 一定距離で消える（下に落ち切ったら）
   /* if (position_.y < -50.0f) {
        isActive_ = false;
    }*/

    worldTransform_.translation_ = position_;
    worldTransform_.UpdateMatrix();
    worldTransform_.TransferMatrix();
}

void EnemyBullet::Draw(Camera* camera) {
    if (isActive_) {
        model_->Draw(worldTransform_, *camera);
    }
}
