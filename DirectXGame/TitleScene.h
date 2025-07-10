#pragma once


#include <cstdint>
#include <KamataEngine.h>
#include "Stage.h"


class TitleScene
{
public:
	TitleScene();

	~TitleScene();

	void Initialize();

	void Update();

	void Draw();
private:
	// タイトルテクスチャハンドル
	uint32_t TitleTextureHandle_ = 0;
	// タイトルスプライト
	KamataEngine::Sprite* TitleSprite_ = nullptr;

	// タイトル操作テクスチャハンドル
	uint32_t HitTextureHandle_ = 0;
	// タイトル操作スプライト
	KamataEngine::Sprite* HitSprite_ = nullptr;

	Stage* stage = nullptr;

	float titleY_ = -100.0f;             // 初期Y座標（画面上からスタート）
	float targetY_ = 200.0f;             // 到達Y座標（真ん中）
	float fallSpeed_ = 5.0f;             // Y方向のスピード
	float oscillationTime_ = 0.0f;       // 揺れ用の時間
	bool isTitleStopped_ = false;        // 停止フラグ
	bool isTitleMoving_ = false;

	float hitAlphaTime_ = 0.0f;
};

