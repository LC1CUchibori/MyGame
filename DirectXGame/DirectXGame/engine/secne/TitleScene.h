#pragma once


#include <cstdint>
#include <KamataEngine.h>
#include "../../applikation/Stage.h"
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
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::WorldTransform worldTransform_;
	// カメラ
	KamataEngine::Camera camera_;

	// タイトル操作テクスチャハンドル
	uint32_t HitTextureHandle_ = 0;
	// タイトル操作スプライト
	KamataEngine::Sprite* HitSprite_ = nullptr;

	Stage* stage1 = nullptr;

	TitleModel* title_ = nullptr;
	KamataEngine::Model* titleModel_ = nullptr;

	float hitAlphaTime_ = 0.0f;
};

