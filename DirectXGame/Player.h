#pragma once
#include <KamataEngine.h>
#include "Model2.h"

class Player
{
public:
	Player();
	~Player();
	void Initialize(KamataEngine::Model* model,uint32_t textureHandle,KamataEngine::Camera* camera);

	void Update();

	void Draw(KamataEngine::Camera* camera,uint32_t textureHandle);
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

	KamataEngine::Vector3 position_;
};

