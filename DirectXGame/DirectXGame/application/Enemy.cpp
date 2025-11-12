#include "Enemy.h"
#include <cassert>
#include <cmath> // sin, cosなどの揺れ用

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
    stopZ_ = -10.0f;
    stopY_ = 10.0f;

    bulletModel_ = KamataEngine::Model::CreateFromOBJ("Enemy");

    // 新しいフラグ類
    isDead_ = false;
    isEscaping_ = false;
    shakeTimer_ = 0;
    escapeSpeed_ = 2.0f;
}

void Enemy::Update() {

    // 死亡した場合
    if (isDead_) {

        if (shakeTimer_ < 60) {
            // その場で震える（sin波でランダムっぽい揺れ）
            position_.x += std::sin(shakeTimer_ * 0.5f) * 0.3f;
            position_.y += std::cos(shakeTimer_ * 0.7f) * 0.3f;
            shakeTimer_++;
        }
        else {
            // 一定時間経ったら上に逃げる
            position_.y += escapeSpeed_;
        }

        // ワールド更新
        worldTransform_.translation_ = position_;
        worldTransform_.UpdateMatrix();
        worldTransform_.TransferMatrix();
        return;
    }

    // ===== 通常行動 =====
    if (isApproaching_) {
        position_.z -= approachSpeed_;

        float t = (200.0f - position_.z) / (200.0f - stopZ_);
        position_.y = 60.0f + t * (stopY_ - 60.0f);

        if (position_.z <= stopZ_) {
            position_.z = stopZ_;
            position_.y = stopY_;
            isApproaching_ = false;
        }
    }

    // 弾の発射
    fireTimer_++;
    if (fireTimer_ > 120) {
        EnemyBullet* bullet = new EnemyBullet();
        bullet->Initialize(bulletModel_, camera_, position_);
        bullets_.push_back(bullet);
        fireTimer_ = 0;
    }

    // 弾の更新
    for (EnemyBullet* bullet : bullets_) {
        bullet->Update();
    }

    // 無効弾削除
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
    for (EnemyBullet* bullet : bullets_) {
        bullet->Draw(camera);
    }
}

// 外部から死亡を通知する関数を追加
void Enemy::OnDeath() {
    if (!isDead_) {
        isDead_ = true;
        shakeTimer_ = 0;
    }
}
