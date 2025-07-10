#pragma once
#include "KamataEngine.h"
#include "Model2.h"
#include "Stage.h"
#include "Player.h"

class GameScene
{
public:
	GameScene();

	~GameScene();

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

private:
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Input* input = KamataEngine::Input::GetInstance();

	// カメラ
	KamataEngine::Camera camera_;

	// エフェクトモデル描画
	KamataEngine::Model2* effectModel_ = nullptr;

	uint32_t textureHandle_ = 0;

	Player* player_ = nullptr;
	KamataEngine::Model* playerModel_ = nullptr;

	// プレイヤーテクスチャハンドル
	uint32_t playerTextureHandle_ = 0;

	Stage* stage = nullptr;
};

