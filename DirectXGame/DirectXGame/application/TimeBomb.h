#pragma once
#include <KamataEngine.h>
#include "../engine/3d/Model2.h"
#include <vector>

class TimeBomb
{
public:
	TimeBomb();
	~TimeBomb();

	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& pos);

	void Update();

	void Draw(KamataEngine::Camera* camera);

	bool IsAlive()const { return status_.isAlive_; }

	bool IsExplode()const { return status_.exploded_; }

	bool GetScale() const { return scale_; }

	void Kill();

	KamataEngine::Vector3 GetPosition()const { return status_.position_; }

private:
	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;
	// モデル
	KamataEngine::Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// カメラ
	KamataEngine::Camera* camera_ = nullptr;

	struct BombStatus {
		KamataEngine::Vector3 position_{};
		int timer_ = 0;
		bool isAlive_ = true;
		bool exploded_ = false;
		int explodeTimer_ = 0;
	};
	BombStatus status_;

	const float explodeTime_ = 300.0f;

	float scale_ = 1.0f;
};

