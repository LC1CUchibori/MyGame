#include "Graph.h"
#include <algorithm> 
using namespace KamataEngine;

Graph::Graph()
{
}

Graph::~Graph()
{
	delete RedSprite_;
	delete GreenSprite_;
}

void Graph::Initialize()
{
	RedSprite_ = new Sprite;
	RedSprite_->Initialize();
	RedSprite_->SetSize({500.0f,50.0f});
	RedSprite_->SetColor({ 1.0f, 0.0f, 0.0f, 0.0f });
	RedSprite_->SetPosition({ 5.0f,5.0f });

	GreenSprite_ = new Sprite;
	GreenSprite_->Initialize();
	GreenSprite_->SetSize({500.0f,50.0f});
	GreenSprite_->SetColor({ 0.0f, 1.0f, 0.0f, 0.5f });
	GreenSprite_->SetPosition({ 5.0f,5.0f });
}

void Graph::Update()
{
}

void Graph::Draw()
{
	RedSprite_->Draw();
	GreenSprite_->Draw();
}

void Graph::SetSize(const Vector2& size)
{
	size_ = size;
	RedSprite_->SetSize(size_);
	GreenSprite_->SetSize(size_);
}

void Graph::SetPosition(const Vector2& pos)
{
	position_ = pos;
	GreenSprite_->SetPosition(position_);
}

void Graph::SetColor(const KamataEngine::Vector4& color)
{
	GreenSprite_->SetColor(color);
}

void Graph::SetGraphValue(float rate)
{
	// rate: 0.0f ～ 1.0f の範囲で受け取る（1.0f で最大長、0.0f で最小）

	// 安全のため clamp（必要なら）
	rate = std::clamp(rate, 0.0f, 1.0f);

	// サイズを rate に応じて変える（幅だけ）
	Vector2 newSize = size_;
	newSize.x = size_.x * rate;
	GreenSprite_->SetSize(newSize);

	// 位置は右から縮むように調整（左端固定）
	// ex: 初期位置が {5, 5} で width が縮まるなら、左端を {5,5} に維持
	GreenSprite_->SetPosition(position_);
}
