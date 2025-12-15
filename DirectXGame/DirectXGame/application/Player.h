#pragma once
#include <KamataEngine.h>
#include <vector>
#include "PlayerBullet.h"
#include "Particle.h"

class Player {
public:
    Player();

    ~Player();

    void Initialize(KamataEngine::Model* mode, KamataEngine::Camera* camera);

    void Update();

    void Draw(KamataEngine::Camera* camera, uint32_t textureHandle);

    void SetYaw(float yaw);

    void Fire();

    // 死亡処理
    void Kill();

    void SetCanShoot(bool canShoot) { canShoot_ = canShoot; }

    void DrawImGui();

    void PlayerMove();

    void BulletUpdate();


    bool CanShoot() const { return canShoot_; }
    bool IsDead() const { return isDead_; }

    // プレイヤー弾Getter
    const std::vector<PlayerBullet*>& GetBullets() const { return bullets_; }
    // 座標取得Getter
    const KamataEngine::Vector3& GetPosition() const { return worldTransform_.translation_; }

    // ワールド座標 → スクリーン座標
    KamataEngine::Vector2 WorldToScreen(const KamataEngine::Vector3& worldPos, KamataEngine::Camera* camera);

    KamataEngine::Vector3 ScreenToWorld(int screenX, int screenY, float z, KamataEngine::Camera* camera);
   
private:
    // モデル
    KamataEngine::Model* model_ = nullptr;
    // カメラ
    KamataEngine::Camera* camera_ = nullptr;
    uint32_t textureHandle_ = 0;
    // ポジション
    KamataEngine::Vector3 position_{};
    // ワールドトランスフォーム
    KamataEngine::WorldTransform worldTransform_;
    // インプット
    KamataEngine::Input* input = nullptr;
    // マウスカーソル
    KamataEngine::Input::MouseMove GetMouseMove();

    float mousePosX = 640.0f;
    float mousePosY = 360.0f;

    // プレイヤー弾
    std::vector<PlayerBullet*> bullets_;
    KamataEngine::Model* bulletModel_ = nullptr;

    // プレイヤーパーティクル
    std::vector<Particle*> particles_;
    KamataEngine::Model* particleModel_ = nullptr;

    // 移動速度
    float speed_ = 0.2f;
    // 回転
    float yaw_ = 0.0f;

    KamataEngine::Vector3 targetPos_;   // 目標位置
    float moveLerp_ = 0.15f; // 補間係数

    // フラグ類
    bool isDead_ = false;  // 死亡フラグ
    bool canShoot_ = true; // 発射フラグ

    // 追尾処理変数など
    float dx;
    float dy;
    float angle;
    POINT mousePos;
};
