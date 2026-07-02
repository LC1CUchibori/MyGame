#include "EnemyAttackState.h"
#include "EnemyMoveState.h"

void EnemyAttackState::Update(Enemy* enemy)
{
    if (enemy->dash_.isDashing && enemy->targetPos_) {

        float dashSpeed = 0.5f;

        // --- プレイヤー方向へ直線突進 ---
        enemy->position_.x += enemy->direction.x * dashSpeed;
        enemy->position_.y += enemy->direction.y * dashSpeed;
        enemy->position_.z += enemy->direction.z * dashSpeed;

        // ===== 爆弾設置処理 =====
        enemy->bomb_.bombTimer_++;

        if (enemy->bomb_.bombTimer_ >= enemy->bomb_.bombInterval_) {
            enemy->bomb_.requestBomb_ = true; // GameScene に通知
            enemy->bomb_.bombTimer_ = 0;
        }

        bool clampX = false;
        bool clampY = false;

        if (enemy->position_.x > 25.0f) { enemy->position_.x = 25.0f;  clampX = (enemy->direction.x > 0); }
        if (enemy->position_.x < -25.0f){ enemy->position_.x = -25.0f; clampX = (enemy->direction.x < 0); }

        if (enemy->position_.y > 13.5f) { enemy->position_.y = 13.5f;  clampY = (enemy->direction.y > 0); }
        if (enemy->position_.y < -13.5f){ enemy->position_.y = -13.5f; clampY = (enemy->direction.y < 0); }

        if (clampX || clampY) {

            enemy->dash_.isDashing = false;
            enemy->bomb_.bombTimer_ = 0;

            if (enemy->currentDashCount_ < enemy->maxDashCount_) {
                // ===== 連続突進 =====
                enemy->ChangeState(new EnemyMoveState());
                enemy->dash_.stateTimer_ = 300;    // すぐ次の突進に入れる
            }
            else {
                // ===== 突進終了 =====
                enemy->ChangeState(new EnemyMoveState());
                enemy->dash_.stateTimer_ = 0;
                enemy->currentDashCount_ = 0;
                enemy->bomb_.requestBomb_ = false;
            }
            enemy->dash_.justFinished_ = true;

            if (clampX) {
                // ---左右の壁にぶつかった時の上下移動 ---
                enemy->move_.verticalOnly_ = true;
                enemy->move_.direction_ = (enemy->direction.y >= 0) ? 1.0f : -1.0f;
            }
            if (clampY) {
                // --- 上下の壁にぶつかった時の左右移動 ---
                enemy->move_.verticalOnly_ = false;
                enemy->move_.direction_ = (enemy->direction.x >= 0) ? 1.0f : -1.0f;
            }

            return;
        }
    }
}
