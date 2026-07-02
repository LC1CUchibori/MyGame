#include "Enemy.h"
#include <cassert>
#include <cmath>
#include "TimeBomb.h"
#include "EnemyMoveState.h"
#include "EnemyAttackState.h"

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

    state_ = new EnemyMoveState();
}

void Enemy::Update() {

    if (enemy_ && player_ && sharkTop_ && !(player_->IsDead() && sharkTop_->HasReturned())) {
        return;
    }

    // --- 前フレーム位置を保存 ---
    KamataEngine::Vector3 prevPos = position_;

    // --- モデルのスケール ---
    worldTransform_.scale_ = { 2.0f, 2.0f, 2.0f };

    //SetState();

    state_->Update(this);

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
    ChangeState(new EnemyMoveState());

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

void Enemy::ChangeState(EnemyState* state)
{
    delete state_;
    state_ = state;
}
