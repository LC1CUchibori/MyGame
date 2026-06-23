#include "Enemy.h"
#include <cassert>
#include <cmath>
#include "TimeBomb.h"

Enemy::Enemy() {}

Enemy::~Enemy() {

}

void Enemy::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera) {
    assert(model);

    model_ = model;
    camera_ = camera;

    // パーティクル
    particleModel_ = KamataEngine::Model::CreateFromOBJ("ParticleBall");

    // --- 初期位置 ---
    position_ = { 0.0f, 60.0f, 200.0f };

    worldTransform_.Initialize();
    worldTransform_.translation_ = position_;
    worldTransform_.UpdateMatrix();
    worldTransform_.TransferMatrix();

    // --- 移動基本設定 ---
    move_.speed_         = 0.2f;   // 通常の左右移動速度
    move_.approachSpeed_ = 0.8f;   // 最初にZ方向へ降りてくる速度
    move_.direction_     = 1.0f;   // 左右移動方向
    move_.isApproaching_ = true;   // 上空から接近中か
    move_.stopZ_         = -10.0f; // 接近完了位置(Z)
    move_.stopY_         = 10.0f;  // 接近完了位置(Y)
    move_.verticalOnly_  = false;  // 上下のみ移動する特殊モード

    // --- 状態フラグ ---
    isDead_      = false;
    isEscaping_  = false;
    shakeTimer_  = 0;
    move_.escapeSpeed_ = 2.0f; // 死亡後に画面外へ飛んでいく速度

    // --- 突進関連 ---
    dash_.stateTimer_    = 0;    // 次の突進までの経過時間
    dash_.isDashing      = false;
    dash_.duration       = 60;   // 突進の最大継続フレーム
    dash_.preShakeTimer_ = 0;    // 突進前の震え演出
    dash_.justFinished_  = false;


    hp_ = 10;
    maxHp_ = 10;

}

void Enemy::Update() {

    if (enemy_ && player_ && sharkTop_ && !(player_->IsDead() && sharkTop_->HasReturned())) {
        return;
    }

    // --- 前フレーム位置を保存 ---
    KamataEngine::Vector3 prevPos = position_;

    // --- モデルのスケール ---
    worldTransform_.scale_ = { 2.0f, 2.0f, 2.0f };

    SetState();

    // --- パーティクル処理 ---
    ParticleUpdate();

    // --- ワールド行列更新 ---
    worldTransform_.translation_ = position_;

    if (move_.isApproaching_) {
        worldTransform_.rotation_.x = 3.14f / 2.0f;
        worldTransform_.rotation_.z = 3.14f;
    }
    else {
        if (targetPos_) {
            float dx = targetPos_->x - position_.x;
            float dy = targetPos_->y - position_.y;
            float angle = std::atan2(dy, dx);
            worldTransform_.rotation_.z = (angle + 3.14f * 1.5f) * (-1.0f);
        }
        worldTransform_.rotation_.x = 0.0f;
    }

    worldTransform_.UpdateMatrix();
    worldTransform_.TransferMatrix();
}


void Enemy::Draw(KamataEngine::Camera* camera) {

   /* if (!player_) {
        return;
    }

    if (player_->IsDead()) {
        return;
    }

    if (isDead_) {
        return;
    }*/


    // エネミー本体を先に描画
    model_->Draw(worldTransform_, *camera);

    // パーティクルを後に描画（エネミーの後ろに表示）
    for (auto* particle_ : particles_) {
        particle_->Draw();
    }
}

// --- 敵撃破処理 ---
void Enemy::OnDeath() {
    if (!isDead_) {
        isDead_ = true;
        shakeTimer_ = 0;
    }
}

void Enemy::SetAlive() {
    isDead_ = false;
    shakeTimer_ = 0;
}

void Enemy::TakeDamage(int damage)
{
    if (isDead_) return;

    hp_ -= damage;

    if (hp_ <= 0) {
        hp_ = 0;
        OnDeath();
    }
}

void Enemy::SetState() {

    switch (state_) {

        // =============================
        //  移動ステート
        // =============================
    case State::move:

        // --- 死亡演出 ---
        if (isDead_) {
            // 最初は震える
            if (shakeTimer_ < 60) {
                position_.x += std::sin(shakeTimer_ * 0.5f) * 0.3f;
                position_.y += std::cos(shakeTimer_ * 0.7f) * 0.3f;
                shakeTimer_++;
            }
            // 震え後は上方向へ逃げる
            else {
                position_.y += move_.escapeSpeed_;
            }
            return;
        }

        // --- 突進開始条件 ---
        if (!forceMove_ && dash_.stateTimer_ >= 300 && !dash_.isDashing) {

            // 突進前の「震え予兆」
            if (dash_.preShakeTimer_ < 20) {
                position_.x += std::sin(dash_.preShakeTimer_ * 0.5f) * 0.3f;
                position_.y += std::cos(dash_.preShakeTimer_ * 0.7f) * 0.3f;
                dash_.preShakeTimer_++;
            }
            // 震え終わったら突進開始
            else {
                state_ = State::attack;
                dash_.isDashing     = true;
                dash_.justFinished_ = false;
                currentDashCount_++; 

                // --- 突進方向をプレイヤーへ向けて正規化 ---
                if (targetPos_) {
                    direction.x = targetPos_->x - position_.x;
                    direction.y = targetPos_->y - position_.y;
                    direction.z = 0.0f;

                    float len = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                    if (len > 0.001f) {
                        direction.x /= len;
                        direction.y /= len;
                    }
                }

                dash_.preShakeTimer_ = 0;
            }
        }

        // --- 上空からの接近シーン ---
        if (move_.isApproaching_) {

            position_.z -= move_.approachSpeed_;

            // --- Y位置も線形補間で下げる ---
            float t = (200.0f - position_.z) / (200.0f - move_.stopZ_);
            position_.y = 60.0f + t * (move_.stopY_ - 60.0f);

            // --- 接近完了 ---
            if (position_.z <= move_.stopZ_) {
                position_.z = move_.stopZ_;
                position_.y = move_.stopY_;
                move_.isApproaching_ = false;
            }
        }
        else {
            // --- 突進待ち時間カウント ---
            dash_.stateTimer_++;

            // --- 通常移動 ---
            if (!dash_.isDashing) {

                if (!move_.verticalOnly_) {
                    // --- 左右移動 ---
                    position_.x += move_.speed_ * move_.direction_;

                    if (position_.x >= 23.0f) { 
                        position_.x = 23.0f; 
                        move_.direction_ = -1.0f; 
                    }
                    if (position_.x <= -23.0f) { 
                        position_.x = -23.0f; 
                        move_.direction_ = 1.0f; 
                    }
                }
                else {
                    // --- 上下移動 --- 
                    position_.y += move_.speed_ * move_.direction_;

                    if (position_.y >= 11.5f) {
                        position_.y = 11.5f;
                        move_.direction_ = -1.0f;
                    }
                    if (position_.y <= -11.5f) {
                        position_.y = -11.5f;
                        move_.direction_ = 1.0f;
                    }
                }
            }
        }
        break;

        // =============================
        //  攻撃ステート
        // =============================
    case State::attack:

        if (dash_.isDashing && targetPos_) {

            float dashSpeed = 0.5f;

            // --- プレイヤー方向へ直線突進 ---
            position_.x += direction.x * dashSpeed;
            position_.y += direction.y * dashSpeed;
            position_.z += direction.z * dashSpeed;

            // ===== 爆弾設置処理 =====
            bomb_.bombTimer_++;

            if (bomb_.bombTimer_ >= bomb_.bombInterval_) {
                bomb_.requestBomb_ = true; // GameScene に通知
                bomb_.bombTimer_ = 0;
            }

            bool clampX = false;
            bool clampY = false;

            if (position_.x > 25.0f) { position_.x = 25.0f;  clampX = (direction.x > 0); }
            if (position_.x < -25.0f){ position_.x = -25.0f; clampX = (direction.x < 0); }

            if (position_.y > 13.5f) { position_.y = 13.5f;  clampY = (direction.y > 0); }
            if (position_.y < -13.5f){ position_.y = -13.5f; clampY = (direction.y < 0); }

            if (clampX || clampY) {

                dash_.isDashing = false;
                bomb_.bombTimer_ = 0;

                if (currentDashCount_ < maxDashCount_) {
                    // ===== 連続突進 =====
                    state_ = State::move; // 少しmoveにする
                    dash_.stateTimer_ = 300;    // すぐ次の突進に入れる
                }
                else {
                    // ===== 突進終了 =====
                    state_ = State::move;
                    dash_.stateTimer_ = 0;
                    currentDashCount_ = 0;
                    bomb_.requestBomb_ = false;
                }
                dash_.justFinished_ = true;

                if (clampX) {
                    // ---左右の壁にぶつかった時の上下移動 ---
                    move_.verticalOnly_ = true;
                    move_.direction_ = (direction.y >= 0) ? 1.0f : -1.0f;
                }
                if (clampY) {
                    // --- 上下の壁にぶつかった時の左右移動 ---
                    move_.verticalOnly_ = false;
                    move_.direction_ = (direction.x >= 0) ? 1.0f : -1.0f;
                }

                return;
            }
        }
        break;

    default:
        break;
    }
}

void Enemy::ResetDashTimer()
{
    // ===== 突進状態リセット =====
    dash_.isDashing = false;
    dash_.cooldownTimer = 0;
    dash_.stateTimer_ = 0;
    dash_.justFinished_ = false;

    // 突進前の震え
    dash_.isPreDashShaking_ = false;
    dash_.preShakeTimer_ = 0.0f;

    // ===== 突進回数リセット =====
    currentDashCount_ = 0;

    // ===== ステートを安全側に =====
    state_ = State::move;

    // ===== 移動も戻す（重要）=====
    move_.isApproaching_ = true;
}

void Enemy::ParticleUpdate()
{
    // ========== パーティクル処理（後方オフセット） ==========
    {
        Particle* p = new Particle();
        // パーティクルをエネミーの後ろ（奥）に生成
        KamataEngine::Vector3 particlePos = worldTransform_.translation_;
        particlePos.z += 5.0f;  // カメラから見て奥にオフセット
        p->Initialize(particleModel_, camera_, particlePos);
        particles_.push_back(p);
    }

    for (auto* p : particles_) {
        p->Update();
    }

    particles_.erase(
        std::remove_if(particles_.begin(), particles_.end(),
            [](Particle* p) {
                if (!p->IsAlive()) {
                    delete p;
                    return true;
                }
                return false;
            }),
        particles_.end());
    // =========================================================
}
