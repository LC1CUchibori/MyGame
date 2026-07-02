#include "EnemyMoveState.h"
#include "EnemyAttackState.h"

void EnemyMoveState::Update(Enemy* enemy)
{
    // --- 死亡演出 ---
    if (enemy->isDead_) {
        // 最初は震える
        if (enemy->shakeTimer_ < 60) {
            enemy->position_.x += std::sin(enemy->shakeTimer_ * 0.5f) * 0.3f;
            enemy->position_.y += std::cos(enemy->shakeTimer_ * 0.7f) * 0.3f;
            enemy->shakeTimer_++;
        }
        // 震え後は上方向へ逃げる
        else {
            enemy->position_.y += enemy->move_.escapeSpeed_;
        }
        return;
    }

    // --- 突進開始条件 ---
    if (!enemy->forceMove_ && enemy->dash_.stateTimer_ >= 300 && !enemy->dash_.isDashing) {

        // 突進前の「震え予兆」
        if (enemy->dash_.preShakeTimer_ < 20) {
            enemy->position_.x += std::sin(enemy->dash_.preShakeTimer_ * 0.5f) * 0.3f;
            enemy->position_.y += std::cos(enemy->dash_.preShakeTimer_ * 0.7f) * 0.3f;
            enemy->dash_.preShakeTimer_++;
        }
        // 震え終わったら突進開始
        else {
            enemy->ChangeState(new EnemyAttackState());
            enemy->dash_.isDashing     = true;
            enemy->dash_.justFinished_ = false;
            enemy->currentDashCount_++; 

            // --- 突進方向をプレイヤーへ向けて正規化 ---
            if (enemy->targetPos_) {
                enemy->direction.x = enemy->targetPos_->x - enemy-> position_.x;
                enemy->direction.y = enemy->targetPos_->y - enemy->position_.y;
                enemy->direction.z = 0.0f;

                float len = std::sqrt(enemy->direction.x * enemy->direction.x + enemy->direction.y * enemy->direction.y);
                if (len > 0.001f) {
                    enemy->direction.x /= len;
                    enemy->direction.y /= len;
                }
            }

            enemy->dash_.preShakeTimer_ = 0;
        }
    }

    // --- 上空からの接近シーン ---
    if (enemy->move_.isApproaching_) {

        enemy->position_.z -= enemy->move_.approachSpeed_;

        // --- Y位置も線形補間で下げる ---
        float t = (200.0f - enemy->position_.z) / (200.0f - enemy->move_.stopZ_);
        enemy->position_.y = 60.0f + t * (enemy->move_.stopY_ - 60.0f);

        // --- 接近完了 ---
        if (enemy->position_.z <= enemy->move_.stopZ_) {
            enemy->position_.z = enemy->move_.stopZ_;
            enemy->position_.y = enemy->move_.stopY_;
            enemy->move_.isApproaching_ = false;
        }
    }
    else {
        // --- 突進待ち時間カウント ---
        enemy->dash_.stateTimer_++;

        // --- 通常移動 ---
        if (!enemy->dash_.isDashing) {

            if (!enemy->move_.verticalOnly_) {
                // --- 左右移動 ---
                enemy->position_.x += enemy->move_.speed_ * enemy->move_.direction_;

                if (enemy->position_.x >= 23.0f) { 
                    enemy->position_.x = 23.0f; 
                    enemy->move_.direction_ = -1.0f; 
                }
                if (enemy->position_.x <= -23.0f) { 
                    enemy->position_.x = -23.0f; 
                    enemy->move_.direction_ = 1.0f; 
                }
            }
            else {
                // --- 上下移動 --- 
                enemy->position_.y += enemy->move_.speed_ * enemy->move_.direction_;

                if (enemy->position_.y >= 11.5f) {
                    enemy->position_.y = 11.5f;
                    enemy->move_.direction_ = -1.0f;
                }
                if (enemy->position_.y <= -11.5f) {
                    enemy->position_.y = -11.5f;
                    enemy->move_.direction_ = 1.0f;
                }
            }
        }
    }
}
