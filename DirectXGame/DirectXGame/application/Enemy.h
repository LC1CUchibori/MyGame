#pragma once
#include <KamataEngine.h>
#include "../engine/3d/Model2.h"
#include <vector>
#include "EnemyBullet.h"
#include <vector>
#include "Particle.h"


class Enemy
{
public:
	Enemy();
	~Enemy();

	// 初期化
	void Initialize(KamataEngine::Model* model,KamataEngine::Camera* camera);
	
	// 更新
	void Update();
	
	// 描画
	void Draw(KamataEngine::Camera* camera);
	
	// スピードについて
	void SetSpeed(float speed) {
		move_.speed_ = speed; 
	}
	
	// ポジションについて
	void SetDirection(float dir) {
		move_.direction_ = dir; 
	}
	
	// 倒されているか
	void OnDeath(); 
	
	// 生きているか
	void SetAlive();
	
	// ダメージを与える
	void TakeDamage(int damage);

	// State関数
	void SetState();

	// 接近フラグ
	void ResetApproach() {
		move_.isApproaching_ = true;
	}

	// 動くかどうか
	void SetInactive() {
		move_.isActive_ = false;
	}

	// ポジションセット
	void SetPosition(const KamataEngine::Vector3& pos) {
		position_ = pos;
	}

	// 弾を打つか
	void SetCanShoot(bool canShoot) {
		canShoot_ = canShoot; 
	}

	void SetForceMove(bool flag) { forceMove_ = flag; }

	// フェーズリセット
	void ResetForPhase(const KamataEngine::Vector3& pos) {
		position_ = pos;
		hp_ = 10;
		maxHp_ = 10;
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

	// 突進するカウント関数
	void SetMaxDashCount(int count) {
		maxDashCount_ = count;
	}

	void ResetRequestBomb() { bomb_.requestBomb_ = false; }

	void ResetDashTimer();

	void ParticleUpdate();

	bool CanShoot() const {
		return canShoot_; 
	}

	bool HasEscaped() const {
		return isDead_ && position_.y > 150.0f;
	}

	bool IsDead() const {
		return isDead_; 
	}

	bool IsApproachFinished() const {
		return !move_.isApproaching_;
	}

	const KamataEngine::Vector3& GetPosition() const {
		return position_;
	}

	int GetHp() const { return hp_; }
	int GetMaxHp() const { return maxHp_; }

	bool IsRequestBomb() const { return bomb_.requestBomb_; }

	


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
		int cooldown = 300;          // 突進クールダウン
		int cooldownTimer = 0;       // クールダウン用タイマー
		int stateTimer_ = 0;         // 5秒タイマー
		bool justFinished_ = false;

		bool isPreDashShaking_ = false;
		float preShakeTimer_ = 0.0f;
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

	struct Bomb
	{
		float bombTimer_ = 0.0f;
		float bombInterval_ = 30.0f; 
		bool requestBomb_ = false;
	};
	Bomb bomb_;

	// state構造体
	enum class State:uint8_t
	{
		move,   // 移動
		attack, // 突進
	};
	State state_;

	KamataEngine::Vector3 direction;

	// エネミーパーティクル
	std::vector<Particle*> particles_;
	KamataEngine::Model* particleModel_ = nullptr;

	// その他タイマー
	int shakeTimer_ = 0;

	bool forceMove_ = false; 

	int maxDashCount_ = 1;   // 最大突進回数
	int currentDashCount_ = 0; // 今何回目の突進か

	float moveLerp_ = 0.15f; // 補間係数

	int hp_ = 0;
	int maxHp_ = 0;

	// その他フラグ類
	bool isDead_ = false;
	bool isEscaping_ = false;
	bool canShoot_ = true;
	
};

