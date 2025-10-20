#pragma once
#include "KamataEngine.h"
#include "Model2.h"
#include "Stage.h"
#include "Player.h"
#include "Enemy.h"
#include <cstdlib>
#include <ctime>  
#include "CrossHair3D.h"

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

	// プレイヤー
	Player* player_ = nullptr;
	KamataEngine::Model* playerModel_ = nullptr;
	uint32_t playerTextureHandle_ = 0;

	// 敵
	std::vector<Enemy*> enemies_; // 複数の敵
	KamataEngine::Model* enemyModel_ = nullptr;
	uint32_t enemyTextureHandle_ = 0;

	Stage* stage = nullptr;

	KamataEngine::Vector3 cursorPos_{0.0f, 0.0f, 30.0f}; // 初期位置
	float cursorSpeed_ = 0.5f; // 移動速度

	uint32_t crossTexture_;// 標準
	KamataEngine::Sprite* cursorSprite_ = nullptr;
	float cursorX_ = 400.0f; // 初期位置（画面中央など）
	float cursorY_ = 300.0f;

	Crosshair3D* crosshair_ = nullptr;
	KamataEngine::Model* crosshairModel_ = nullptr;
};

