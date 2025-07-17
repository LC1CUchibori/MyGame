#pragma once
#include <KamataEngine.h>
#include "Model2.h"
#include "Stage.h"

class Player
{
public:
	Player();
	~Player();
	void Initialize(KamataEngine::Model* model,uint32_t textureHandle,KamataEngine::Camera* camera);

	void Update();

	void Draw(KamataEngine::Camera* camera,uint32_t textureHandle);

	void SetStage(Stage* stage);
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

	Stage* stage_ = nullptr;

	bool isDescending_ = true;
	float descendSpeed_ = 0.5f; // 落下速度
};

