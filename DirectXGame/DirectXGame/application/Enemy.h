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

	void SetSpeed(float speed) {
		move_.speed_ = speed; 
	}

	void SetDirection(float dir) {
		move_.direction_ = dir; 
	}

	void OnDeath(); 

	void SetAlive();

	// State関数
	void SetState();

	void ResetApproach() {
		move_.isApproaching_ = true;
	}

	void SetInactive() {
		move_.isActive_ = false;
	}

	void SetPosition(const KamataEngine::Vector3& pos) {
		position_ = pos;
	}

	void SetCanShoot(bool canShoot) {
		canShoot_ = canShoot; 
	}

	void SetForceMove(bool flag) { forceMove_ = flag; }

	void ResetForPhase(const KamataEngine::Vector3& pos) {
		position_ = pos;
		isDead_ = false;
		isEscaping_ = false;
		shakeTimer_ = 0;
		move_.isApproaching_ = true;
		worldTransform_.translation_ = position_;
		worldTransform_.UpdateMatrix();
		worldTransform_.TransferMatrix();
	}

	// 動き止める関数
	void StopMoving() {
		move_.isApproaching_ = false;
		move_.speed_ = 0;
		move_.approachSpeed_ = 0;
	}

	// ターゲットのポジション
	void SetTarget(const KamataEngine::Vector3* target) {
		targetPos_ = target; 
	}

	bool CanShoot() const {
		return canShoot_; 
	}

	bool HasEscaped() const {
		return isDead_ && position_.y > 150.0f;
	}

	bool IsDead() const {
		return isDead_; 
	}

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
	// ポジション
	KamataEngine::Vector3 position_;
	// プレイヤーの位置
	const KamataEngine::Vector3* targetPos_ = nullptr;

	// 突進構造体
	struct DashData {
		bool isDashing = false;      // 突進中フラグ
		KamataEngine::Vector3 dir;   // 突進方向
		float speed = 1.5f;          // 突進速度
		int duration = 60;           // 突進持続フレーム
		int timer = 0;               // 突進タイマー
		int cooldown = 300;          // 突進クールダウン
		int cooldownTimer = 0;       // クールダウン用タイマー
		int stateTimer_ = 0;         // 5秒タイマー
		bool justFinished_ = false;
	};
	DashData dash_;                  // 突進データ

	// 移動範囲構造体
	struct ScreenRang
	{
		float minX = -20.0f;      // 左端
		float maxX = 20.0f;       // 右端
		float minY = 0.0f;        // 下端
		float maxY = 20.0f;       // 上端
	};
	ScreenRang screenRang_;

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

	// state構造体
	enum State
	{
		move,   // 移動
		attack, // 突進
	};
	State state_;

	KamataEngine::Vector3 direction;

	// その他タイマー
	int shakeTimer_ = 0;

	bool forceMove_ = false; 

	// その他フラグ類
	bool isDead_ = false;
	bool isEscaping_ = false;
	bool canShoot_ = true;
};

