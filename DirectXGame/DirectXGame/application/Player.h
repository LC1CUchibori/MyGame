#pragma once
#include <KamataEngine.h>
#include "../engine/3d/Model2.h"
#include "Stage.h"

class Player
{
public:
	Player();
	~Player();
	void Initialize(KamataEngine::Model* model,uint32_t textureHandle,KamataEngine::Camera* camera);

	void Update();

	void Draw(KamataEngine::Camera* camera,uint32_t textureHandle);

	void SetYaw(float yaw);

	KamataEngine::Vector3 GetPosition() const { return position_; }

private:
	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Input* input = KamataEngine::Input::GetInstance();
	// モデル
	KamataEngine::Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// カメラ
	KamataEngine::Camera* camera_ = nullptr;
	// 位置
	KamataEngine::Vector3 position_;

	// Y軸回転
	float  yaw_ = 0.0f;
};

