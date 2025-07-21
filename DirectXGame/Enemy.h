#pragma once
#include <KamataEngine.h>
#include "Model2.h"
#include <vector>

class Enemy
{
public:
	Enemy();
	~Enemy();

	void Initialize(KamataEngine::Model* model,uint32_t textureHandle,KamataEngine::Camera* camera);

	void Update();

	void Draw(KamataEngine::Camera* camera,uint32_t textureHandle);

	void SetPosition(const KamataEngine::Vector3& pos) {
		position_ = pos;
	}

	void SetSpeed(float speed) { speed_ = speed; }

	void SetDirection(int dir) { direction_ = dir; }

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

	float speed_ = 0.2f;
	int direction_ = -1; 
};

