#pragma once


#include <cstdint>
#include <KamataEngine.h>
#include "../../application/Stage.h"
#include "../3d/TitleModel.h"

class TitleScene
{
public:
	TitleScene();

	~TitleScene();

	void Initialize();

	void Update();

	void Draw();
private:
	// モデル
	KamataEngine::Model* model_ = nullptr;
	// ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;
	// カメラ
	KamataEngine::Camera camera_;
	// インプット
	KamataEngine::Input* input = KamataEngine::Input::GetInstance();

	// タイトル操作テクスチャハンドル
	uint32_t HitTextureHandle_ = 0;
	// タイトル操作スプライト
	KamataEngine::Sprite* HitSprite_ = nullptr;

	// タイトルスプライト
	uint32_t TitleTextureHandle_ = 0;
	KamataEngine::Sprite* TitleSprite_ = nullptr;

	// ルール説明
	KamataEngine::Sprite* ruleSprite_ = nullptr;
	uint32_t ruleTextureHandle_ = 0;
	bool isRule_ = false;

	// ステージ
	Stage* stage1 = nullptr;

	// タイトルモデル
	TitleModel* title_ = nullptr;
	KamataEngine::Model* titleModel_ = nullptr;

	float hitAlphaTime_ = 0.0f;
};

