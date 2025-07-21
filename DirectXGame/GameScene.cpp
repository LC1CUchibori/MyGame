#include "GameScene.h"

using namespace KamataEngine;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	delete effectModel_;
	delete model_;
	delete player_;

	Model2::StaticFinalize();
}

void GameScene::Initialize()
{
	// Model2の初期化
	Model2::StaticInitialize();

	// ランダム初期化
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	// プレイヤー
	playerTextureHandle_ = TextureManager::Load("BlackORE.png");
	playerModel_ = Model::Create();
	player_ = new Player();
	player_->Initialize(playerModel_,playerTextureHandle_,&camera_);

	// 敵
	enemyTextureHandle_ = TextureManager::Load("white1x1.png");
	enemyModel_ = Model::Create();

	for (int i = 0; i < 20; ++i) {
		Enemy* enemy = new Enemy();
		enemy->Initialize(playerModel_, playerTextureHandle_, &camera_);

		// 右からか左からかを選別
		int direction = (std::rand() % 2 == 0) ? -1 : 1;

		// 高さをランダム
		float y = -15.0f + static_cast<float>(std::rand()) / RAND_MAX * 50.0f;

		// 左右をランダム
		float x = (direction == -1) ? 40.0f + std::rand() % 20 : -40.0f - std::rand() % 20;

		enemy->SetPosition({ x,y,30.0f});
		enemy->SetDirection(direction);
		enemy->SetSpeed(0.2f+ static_cast<float>(std::rand()) / RAND_MAX * 0.5f);

		enemies_.push_back(enemy);
	}

	stage = new Stage;
	stage->Initialize();


	worldTransform_.Initialize();
	 // カメラの初期化
	camera_.Initialize();
}

void GameScene::Update()
{
	stage->Update();

	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}

	player_->Update();

	worldTransform_.UpdateMatrix();
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void GameScene::Draw()
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

#pragma region 3Dオブジェクト描画
	dxCommn->ClearDepthBuffer();
	// 3Dオブジェクト描画前処理
	Model::PreDraw(dxCommn->GetCommandList());

	player_->Draw(&camera_, playerTextureHandle_);

	for (Enemy* enemy : enemies_) {
		enemy->Draw(&camera_, enemyTextureHandle_);
	}

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion


	// 3Dモデル描画前処理
	Model2::PreDraw(dxCommn->GetCommandList());

	

	// 3Dモデル描画後処理
	Model2::PostDraw();
}
