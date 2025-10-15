#include "Fade.h"
#include <cmath>

using namespace KamataEngine;

void Fade::Initialize()
{
	fadeTextureHandle_=TextureManager::Load("white1x1.png");
	fadeSprite_ = Sprite::Create(fadeTextureHandle_, { 0,0 });
	fadeSprite_->SetSize({1280, 720}); // 画面全体を覆うサイズに
	fadeSprite_->SetColor({1, 1, 1, alpha_}); // 白フェード
	alpha_ = 0.0f;
}

void Fade::Update()
{
	if (!isFading_) return;

	if (isFadeOut_) {
		alpha_ += 0.02f; // フェードアウト速度
		if (alpha_ >= 1.0f) {
			alpha_ = 1.0f;
			fadeOutEnd_ = true;
			isFading_ = false;
		}
	} else {
		alpha_ -= 0.02f; // フェードイン速度
		if (alpha_ <= 0.0f) {
			alpha_ = 0.0f;
			fadeInEnd_ = true;
			isFading_ = false;
		}
	}
}

void Fade::Draw()
{

	// DirectXCommon インスタンスの取得
	DirectXCommon* dxCommn = DirectXCommon::GetInstance();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(dxCommn->GetCommandList());

	if (alpha_ <= 0.0f) return;

	fadeSprite_->SetColor({0, 0, 0, alpha_});
	fadeSprite_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}

void Fade::StartFadeOut()
{
	isFading_ = true;
	isFadeOut_ = true;
	fadeOutEnd_ = false;
	fadeInEnd_ = false;
	alpha_ = 0.0f;
}

void Fade::StartFadeIn()
{
	isFading_ = true;
	isFadeOut_ = false;
	fadeOutEnd_ = false;
	fadeInEnd_ = false;
	alpha_ = 1.0f;
}
