#pragma once
#include <cstdint>
#include <KamataEngine.h>

class Stage
{
public:
	Stage();
	~Stage();

	void Initialize();

	void Update();

	void Draw();

	bool IsScrollFinished() const {
		return totalScroll > 719.0f;
	}

private:
	// 背景テクスチャハンドル
	uint32_t BGTextureHandle_ = 0;
	// 背景スプライト
	KamataEngine::Sprite* BGSprite_ = nullptr;

	// 背景テクスチャハンドル
	uint32_t ScrollBGTextureHandle_ = 0;
	// 背景スプライト
	KamataEngine::Sprite* ScrollBGSprite_ = nullptr;

	const float scrollSpeed = 2.0f;

    float totalScroll = 0.0f;
};

