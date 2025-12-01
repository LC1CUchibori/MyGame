#pragma once
#include <KamataEngine.h>
#include "../engine/3d/Model2.h"
#include <vector>
#include "EnemyBullet.h"
#include <vector>


class Enemy
{
public:
	Enemy();
	~Enemy();

	void Initialize(KamataEngine::Model* model,KamataEngine::Camera* camera);

	void Update();

	void Draw(KamataEngine::Camera* camera);

	void SetSpeed(float speed) { speed_ = speed; }

	void SetDirection(float dir) { direction_ = dir; }

	void OnDeath(); 

	void SetAlive();

	void ResetApproach() {
		isApproaching_ = true;
	}

	void SetInactive() {
		isActive_ = false;
	}

	void SetPosition(const KamataEngine::Vector3& pos) {
		position_ = pos;
	}

	void SetCanShoot(bool canShoot) { canShoot_ = canShoot; }

	bool CanShoot() const { return canShoot_; }

	bool HasEscaped() const {
		return isDead_ && position_.y > 150.0f;
	}

	bool IsDead() const { return isDead_; }

	void ResetForPhase(const KamataEngine::Vector3& pos) {
		position_ = pos;
		isDead_ = false;
		isEscaping_ = false;
		shakeTimer_ = 0;
		isApproaching_ = true;
		worldTransform_.translation_ = position_;
		worldTransform_.UpdateMatrix();
		worldTransform_.TransferMatrix();
	}

	// 動き止める関数
	void StopMoving() {
		isApproaching_ = false;
		speed_ = 0;
		approachSpeed_ = 0;
	}

	// ターゲットのポジション
	void SetTarget(const KamataEngine::Vector3* target) { targetPos_ = target; }

	// State関数
	void SetState();

	const KamataEngine::Vector3& GetPosition() const {
		return position_;
	}

private:
	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;
	// モデル
	KamataEngine::Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// カメラ
	KamataEngine::Camera* camera_ = nullptr;

	KamataEngine::Vector3 position_;

	const KamataEngine::Vector3* targetPos_ = nullptr; // プレイヤーの位置

	struct DashData {
		bool isDashing = false;      // 突進中フラグ
		KamataEngine::Vector3 dir;   // 突進方向
		float speed = 1.5f;          // 突進速度
		int duration = 60;           // 突進持続フレーム
		int timer = 0;               // 突進タイマー
		int cooldown = 300;          // 突進クールダウン
		int cooldownTimer = 0;       // クールダウン用タイマー
	};
	DashData dash_;        // 突進データ
	int stateTimer_ = 0;   // 5秒タイマー

	struct ScreenRang
	{
		float minX = -20.0f;      // 左端
		float maxX = 20.0f;       // 右端
		float minY = 0.0f;        // 下端
		float maxY = 20.0f;       // 上端
	};
	ScreenRang screenRang_;

	float speed_ = 0.2f;
	float direction_ = -1; 
	float approachSpeed_ = 0.0f;
	bool isApproaching_ = true;
	float stopZ_ = -10.0f;
	float stopY_ = 10.0f; 

	bool isActive_;

	bool isDefeated_ = false;
	float defeatTimer_ = 0.0f;

	bool isDead_ = false;
	bool isEscaping_ = false;
	int shakeTimer_ = 0;
	float escapeSpeed_ = 2.0f;

	bool canShoot_ = true;

	enum State
	{
		move,
		attack,
	};

	State state_;

	KamataEngine::Vector3 direction;
};

