#include "TitleScene.h"
#include <cmath>

using namespace KamataEngine;

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
	delete TitleSprite_;
}

void TitleScene::Initialize()
{
	TitleTextureHandle_ = TextureManager::Load("title.png");
	TitleSprite_ = Sprite::Create(TitleTextureHandle_, { 150 + 50, -100 });

	HitTextureHandle_ = TextureManager::Load("Hit.png");
	HitSprite_ = Sprite::Create(HitTextureHandle_, { 170,200 });

	stage = new Stage;
	stage->Initialize();

	titleY_ = -100.0f;
	targetY_ = 50.0f;
	isTitleMoving_ = false;
	isTitleStopped_ = false;
	oscillationTime_ = 0.0f;

	hitAlphaTime_ = 0.0f;
}

void TitleScene::Update()
{
	Input* input = Input::GetInstance();

	// スペースキーが押されたら移動開始
	if (!isTitleMoving_ && input->TriggerKey(DIK_SPACE)) {
		isTitleMoving_ = true;
	}

	if (isTitleMoving_ && !isTitleStopped_) {
		// Y座標のイージング移動
		float deltaY = targetY_ - titleY_;
		titleY_ += deltaY * 0.03f;

		// 揺れの時間更新
		oscillationTime_ += 0.01f;

		// X座標揺れ
		float offsetX = std::sin(oscillationTime_) * 10.0f;

		// 停止条件
		if (std::abs(deltaY) < 0.5f) {
			isTitleStopped_ = true;
			titleY_ = targetY_;
			offsetX = 0.0f;
		}

		// スプライト位置設定（揺れ含む）
		TitleSprite_->SetPosition({ 150 + 50, titleY_ });
	}

	// α値用タイマーを進める
	hitAlphaTime_ += 1.0f / 60.0f;  // 60FPS前提

	// α値をsin波で滑らかに変化（0.0～1.0）
	float alpha = (std::sin(hitAlphaTime_ * 3.14f * 2.0f) + 1.0f) * 0.5f;

	// スプライトに色設定（R,G,B=1.0f, αだけ変化）
	HitSprite_->SetColor({ 1.0f, 1.0f, 1.0f, alpha });


	stage->Update();
}

void TitleScene::Draw()
{
	// DirectXCommon インスタンスの取得
	DirectXCommon* dxCommn = DirectXCommon::GetInstance();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(dxCommn->GetCommandList());

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	stage->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion


	// 3Dモデル描画前処理
	Sprite::PreDraw(dxCommn->GetCommandList());

	TitleSprite_->Draw();
	HitSprite_->Draw();

	// 3Dモデル描画後処理
	Sprite::PostDraw();
}
