#include "TitleScene.h"
#include <cmath>

using namespace KamataEngine;

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
	delete title_;
	delete stage1;
}

void TitleScene::Initialize()
{
	HitTextureHandle_ = TextureManager::Load("Hit.png");
	HitSprite_ = Sprite::Create(HitTextureHandle_, { 170,200 });

	titleModel_ = Model::CreateFromOBJ("Title");
	title_ = new TitleModel();
	title_->Initialize(titleModel_);

	// 背景ステージ
	stage1 = new Stage;
	stage1->Initialize();

	// 点滅タイマー
	hitAlphaTime_ = 0.0f;

	// カメラの初期化
	camera_.Initialize();
}

void TitleScene::Update()
{
	//Input* input = Input::GetInstance();

	// α値用タイマーを進める
	hitAlphaTime_ += 1.0f / 60.0f;  // 60FPS前提

	// α値をsin波で滑らかに変化（0.0～1.0）
	float alpha = (std::sin(hitAlphaTime_ * 3.14f * 2.0f) + 1.0f) * 0.5f;

	// スプライトに色設定（R,G,B=1.0f, αだけ変化）
	HitSprite_->SetColor({ 1.0f, 1.0f, 1.0f, alpha });

	//=======タイトルモデル移動処理===========
	title_->Update();

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

	HitSprite_->Draw();

	// 3Dモデル描画後処理
	Sprite::PostDraw();

#pragma region 3Dオブジェクト描画
	dxCommn->ClearDepthBuffer();
	// 3Dオブジェクト描画前処理
	Model::PreDraw(dxCommn->GetCommandList());

	title_->Draw(&camera_);

	Model::PostDraw();
}
