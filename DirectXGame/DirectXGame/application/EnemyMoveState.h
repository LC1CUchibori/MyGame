#pragma once
#include "EnemyState.h"
#include "KamataEngine.h"
#include "Particle.h"
#include "Enemy.h"

class EnemyMoveState : public EnemyState {
public:
    void Update(Enemy* enemy) override;

private:
    KamataEngine::WorldTransform worldTransform_;
    KamataEngine::Model* model_;
    KamataEngine::Camera* camera_;

    KamataEngine::Vector3 position_;
    const KamataEngine::Vector3* targetPos_;

    int hp_;
    int maxHp_;

    bool isDead_;

    // その他タイマー
    int shakeTimer_ = 0;

    bool forceMove_ = false;

    int currentDashCount_ = 0; // 今何回目の突進か

    std::vector<Particle*> particles_;
    KamataEngine::Model* particleModel_;
    KamataEngine::Vector3 direction;

    // 移動構造体
    struct Move
    {
        float speed_ = 0.2f;            // スピード
        float approachSpeed_ = 0.0f;    // 接近速度
        float escapeSpeed_ = 2.0f;      // 離脱速度
        float direction_ = -1;          // 移動範囲
        float stopZ_ = -10.0f;          // z座標停止位置
        float stopY_ = 10.0f;           // y座標停止位置
        bool isApproaching_ = true;     // 接近フラグ
        bool isActive_;
        bool verticalOnly_ = false;
    };
    Move move_;

    // 突進構造体
    struct DashData {
        bool isDashing = false;      // 突進中フラグ
        KamataEngine::Vector3 dir;   // 突進方向
        float speed = 1.5f;          // 突進速度
        int duration = 60;           // 突進持続フレーム
        int cooldown = 300;          // 突進クールダウン
        int cooldownTimer = 0;       // クールダウン用タイマー
        int stateTimer_ = 0;         // 5秒タイマー
        bool justFinished_ = false;

        bool isPreDashShaking_ = false;
        float preShakeTimer_ = 0.0f;
    };
    DashData dash_;


};