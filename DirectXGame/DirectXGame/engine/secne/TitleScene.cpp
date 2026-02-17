#include "TitleScene.h"
#include <cmath>

using namespace KamataEngine;

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
	// タイトルpushスプライト
	HitTextureHandle_ = TextureManager::Load("hit.png");
	HitSprite_ = Sprite::Create(HitTextureHandle_, { 380,200 });

	// タイトルスプライト
	TitleTextureHandle_ = TextureManager::Load("Title.png");
	TitleSprite_ = Sprite::Create(TitleTextureHandle_, { 380,-50 });

	// ルール説明スプライト
	ruleTextureHandle_ = TextureManager::Load("Rule.png");
	ruleSprite_ = Sprite::Create(ruleTextureHandle_, { 0.0f,0.0f });

	titleModel_.reset(Model::CreateFromOBJ("Title"));
	title_ = std::make_unique<TitleModel>();
	title_->Initialize(titleModel_.get());

	// STARTランプスプライト
	gogoOFFTextureHandle_ = TextureManager::Load("gogoOFF.png");
	gogoOFFSprite_ = Sprite::Create(gogoOFFTextureHandle_, {0, 550});

	gogoONTextureHandle_ = TextureManager::Load("gogoON.png");
	gogoONSprite_ = Sprite::Create(gogoONTextureHandle_, {0, 550});

	// 背景ステージ
	stage1 = std::make_unique<Stage>();
	stage1->Initialize();

	// 点滅タイマー
	hitAlphaTime_ = 0.0f;

	// カメラの初期化
	camera_.Initialize();
}

void TitleScene::Update()
{
	// α値用タイマーを進める
	hitAlphaTime_ += 1.0f / 60.0f;

	// α値をsin波で滑らかに変化
	float alpha = (std::sin(hitAlphaTime_ * 3.14f * 2.0f) + 1.0f) * 0.5f;

	// スプライトに色設定
	HitSprite_->SetColor({ 1.0f, 1.0f, 1.0f, alpha });

	// SPACE押したらON
	if (input->TriggerKey(DIK_SPACE)) {
		isGogoOn_ = true;
	}

	stage1->Update();
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
	stage1->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion


	// 3Dモデル描画前処理
	Sprite::PreDraw(dxCommn->GetCommandList());

	TitleSprite_->Draw();

	HitSprite_->Draw();

	// まを描画
	gogoOFFSprite_->Draw();

	// ON状態なら上から重ねる
	if (isGogoOn_) {
		gogoONSprite_->Draw();
	}

	if (input->TriggerKey(DIK_E)) {
		isRule_ = !isRule_; // トグル（反転）
	}
	if (isRule_) {
		ruleSprite_->Draw(); // 表示
	}

	// 3Dモデル描画後処理
	Sprite::PostDraw();

#pragma region 3Dオブジェクト描画
	dxCommn->ClearDepthBuffer();
	// 3Dオブジェクト描画前処理
	Model::PreDraw(dxCommn->GetCommandList());

	//title_->Draw(&camera_);

	Model::PostDraw();
}
