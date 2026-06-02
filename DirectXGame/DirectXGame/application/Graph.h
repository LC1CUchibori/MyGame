#pragma once
#include "KamataEngine.h"

class Graph
{
public:
	Graph();
	~Graph();

	void Initialize();

	void Update();

	void Draw();

	void SetSize(const KamataEngine::Vector2& size);              // グラフの値（高さ）を設定
	void SetPosition(const KamataEngine::Vector2& pos);
	void SetColor(const KamataEngine::Vector4& color);

	void SetGraphValue(float rate);

private:
	KamataEngine::Sprite* RedSprite_ = nullptr;
	KamataEngine::Sprite* GreenSprite_ = nullptr;
	KamataEngine::Vector2 position_{};
	KamataEngine::Vector2 size_;
};
