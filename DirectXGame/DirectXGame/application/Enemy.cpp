#include "Enemy.h"
#include <cassert>
#include <cmath>

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
    move_.speed_ = 0.2f;       // 左右移動の速度
    move_.approachSpeed_ = 0.8f; // 手前に来る速度
    move_.direction_ = 1.0f;   // 初期は右方向
    move_.isApproaching_ = true; // 最初は接近中
    move_.stopZ_ = -10.0f;
    move_.stopY_ = 10.0f;


    // 新しいフラグ類
    isDead_ = false;
    isEscaping_ = false;
    shakeTimer_ = 0;
    move_.escapeSpeed_ = 2.0f;
}

void Enemy::Update() {

    worldTransform_.scale_.x = 2.0f;
    worldTransform_.scale_.y = 2.0f;
    worldTransform_.scale_.z = 2.0f;

    SetState();
    
    // ワールド変換更新
    worldTransform_.translation_ = position_;
    if (move_.isApproaching_) {
        worldTransform_.rotation_.x = (3.14f / 2.0f);
        worldTransform_.rotation_.z = (3.14f);
    }
    else {
        worldTransform_.rotation_.x = 0.0f;
        worldTransform_.rotation_.z = 0.0f;
    }
    worldTransform_.UpdateMatrix();
    worldTransform_.TransferMatrix();
}

void Enemy::Draw(KamataEngine::Camera* camera) {
    model_->Draw(worldTransform_, *camera);
}


void Enemy::OnDeath() {
    if (!isDead_) {
        isDead_ = true;
        shakeTimer_ = 0;
    }
}

void Enemy::SetAlive()
{
    isDead_ = false;
    shakeTimer_ = 0;
}

void Enemy::SetState()
{
    switch (state_)
    {
    case move:
        if (isDead_) {
            // 死亡時の挙動
            if (shakeTimer_ < 60) {
                position_.x += std::sin(shakeTimer_ * 0.5f) * 0.3f;
                position_.y += std::cos(shakeTimer_ * 0.7f) * 0.3f;
                shakeTimer_++;
            } else {
                position_.y += move_.escapeSpeed_;
            }
            return;
        }

        if (move_.isApproaching_) {
            position_.z -= move_.approachSpeed_;
            float t = (200.0f - position_.z) / (200.0f - move_.stopZ_);
            position_.y = 60.0f + t * (move_.stopY_ - 60.0f);

            if (position_.z <= move_.stopZ_) {
                position_.z = move_.stopZ_;
                position_.y = move_.stopY_;
                move_.isApproaching_ = false;
            }
        } else {
            // 左右移動
            position_.x += move_.speed_ * move_.direction_;
            dash_.stateTimer_++;
            if (position_.x > 30.0f) {
                position_.x = 30.0f; move_.direction_ = -1.0f; 
            }
            else if (position_.x < -30.0f) {
                position_.x = -30.0f; move_.direction_ = 1.0f; 
            }

            // 5秒に一回攻撃
            if (dash_.stateTimer_ >= 300) {
                state_ = attack;
                dash_.isDashing = true;
                dash_.timer = 0;        // 突進用タイマー
                dash_.duration = 60;   // 2秒
                // プレイヤー方向に移動
                direction.x = targetPos_->x - position_.x;
                direction.y = targetPos_->y - position_.y;
                direction.z = 0.0f;
                float length = std::sqrt(direction.x*direction.x + direction.y*direction.y + direction.z*direction.z);
                if (length > 0.001f) {
                    direction.z /= length;
                    direction.x /= length;
                    direction.y /= length;
                }
            }
        }
        break;

    case attack:
        if (dash_.isDashing && targetPos_) {
          
            float dashSpeed = 0.5f; // 突進速度
            position_.x += direction.x * dashSpeed;
            position_.y += direction.y * dashSpeed;
            position_.z += direction.z * dashSpeed;

            bool outOfRange = false;

            // X方向の範囲チェック
            if (position_.x >= 30.0f) {
                position_.x = 30.0f; outOfRange = true; 
            }
            else if (position_.x <= -30.0f) {
                position_.x = -30.0f; outOfRange = true; 
            }
            // Y方向の範囲チェック
            if (position_.y >= 15.0f) {
                position_.y = 15.0f; outOfRange = true; 
            }
            else if (position_.y <= -15.0f) {
                position_.y = -15.0f; outOfRange = true; 
            }

            // 範囲外なら突進終了
            if (outOfRange) {
                dash_.isDashing = false;
                state_ = move;
                dash_.stateTimer_ = 0;
            }
        }
        break;

    default:
        break;
    }
}
