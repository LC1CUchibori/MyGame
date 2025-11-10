#include "Enemy.h"
#include <cassert>

Enemy::Enemy() {
}

Enemy::~Enemy() {
}

void Enemy::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera) {
    assert(model);

    model_ = model;
    camera_ = camera;

    // 敵の初期位置
    position_ = { 0.0f, 60.0f, 200.0f }; 

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

    bulletModel_ = KamataEngine::Model::CreateFromOBJ("Enemy");

}

void Enemy::Update() {
    if (isApproaching_) {
        // Z方向に手前へ移動
        position_.z -= approachSpeed_;

        float t = (200.0f - position_.z) / (200.0f - stopZ_); 
        position_.y = 60.0f + t * (stopY_ - 60.0f);

        // 停止判定
        if (position_.z <= stopZ_) {
            position_.z = stopZ_;
            position_.y = stopY_;
            isApproaching_ = false;
        }
    }
    //else {
    //    // 左右移動
    //    position_.x += speed_ * direction_;
    //    if (position_.x > 20.0f || position_.x < -20.0f) {
    //        direction_ *= -1;
    //    }
    //}

    // 弾の発射管理
    fireTimer_++;
    if (fireTimer_ > 120) { // 2秒おきに撃つ
        EnemyBullet* bullet = new EnemyBullet();
        bullet->Initialize(bulletModel_, camera_, position_);
        bullets_.push_back(bullet);
        fireTimer_ = 0;
    }


    // 弾の更新
    for (EnemyBullet* bullet : bullets_) {
        bullet->Update();
    }

    // 死亡弾の削除
    bullets_.erase(
        std::remove_if(bullets_.begin(), bullets_.end(),
            [](EnemyBullet* b) {
                if (!b->IsActive()) {
                    delete b;
                    return true;
                }
                return false;
            }),
        bullets_.end());


    // ワールド変換更新
    worldTransform_.translation_ = position_;
    worldTransform_.UpdateMatrix();
    worldTransform_.TransferMatrix();
}



void Enemy::Draw(KamataEngine::Camera* camera) {
    model_->Draw(worldTransform_, *camera);

    // 弾描画
    for (EnemyBullet* bullet : bullets_) {
        bullet->Draw(camera);
    }
}
