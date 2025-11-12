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

	void SetPosition(const KamataEngine::Vector3& pos) {
		position_ = pos;
	}

	void SetSpeed(float speed) { speed_ = speed; }

	void SetDirection(float dir) { direction_ = dir; }

	void ResetApproach() {
		isApproaching_ = true;
	}

	void OnDeath(); 

	const KamataEngine::Vector3& GetPosition() const {
		return position_;
	}

	// 敵の弾を取得
	std::vector<EnemyBullet*>& GetBullets() { return bullets_; }

	void SetInactive() {
		isActive_ = false;
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

	std::vector<EnemyBullet*> bullets_;
	int fireTimer_ = 0;
	KamataEngine::Model* bulletModel_ = nullptr;

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
};

