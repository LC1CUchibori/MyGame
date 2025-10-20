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
	delete crosshairModel_;

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

	// ===============================敵の初期化================================
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
	//===================================================================

	// ステージ
	stage = new Stage;
	stage->Initialize();

	// 照準
	crosshairModel_ = Model::CreateFromOBJ("CrossHair"); 
	crosshair_ = new Crosshair3D();
	crosshair_->Initialize(crosshairModel_);


	worldTransform_.Initialize();
	 // カメラの初期化
	camera_.Initialize();
}

void GameScene::Update()
{
	// 背景ステージ
	stage->Update();

	// ==========================敵の処理===========================
	for (Enemy* enemy : enemies_) {
		enemy->Update();

		// 敵の位置を取得
		KamataEngine::Vector3 pos = enemy->GetPosition();

		// 画面外に出たか判定
		if (pos.x < -60.0f || pos.x > 60.0f) {
			// 再出現処理
			int direction = (std::rand() % 2 == 0) ? -1 : 1;

			float y = -15.0f + static_cast<float>(std::rand()) / RAND_MAX * 50.0f;
			float x = (direction == -1) ? 40.0f + std::rand() % 20 : -40.0f - std::rand() % 20;

			enemy->SetPosition({ x, y, 30.0f });
			enemy->SetDirection(direction);
			enemy->SetSpeed(0.2f + static_cast<float>(std::rand()) / RAND_MAX * 0.5f);
		}
	}
    // ===============================================================


	// =========================照準更新===============================
	crosshair_->Update();

	Vector3 crossPos = crosshair_->GetPosition();
	Vector3 playerPos = player_->GetPosition();

	// プレイヤーからクロスヘアまでの差分
	float deltaX = crossPos.x - playerPos.x;
	float deltaZ = crossPos.z - playerPos.z; 

	// X/Z平面での角度
	float targetYaw = atan2f(deltaZ, deltaX); 
	player_->SetYaw(targetYaw);

	if (input->TriggerKey(DIK_SPACE)) {
		for (Enemy* enemy : enemies_) {
			Vector3 enemyPos = enemy->GetPosition();

			float dx = enemyPos.x - crossPos.x;
			float dy = enemyPos.y - crossPos.y;
			float dz = enemyPos.z - crossPos.z;
			float dist = sqrtf(dx*dx + dy*dy + dz*dz);

			if (dist < 3.0f) {
				// 倒されたらリスポーン
				int direction = (std::rand() % 2 == 0) ? -1 : 1;
				float y = -15.0f + static_cast<float>(std::rand()) / RAND_MAX * 50.0f;
				float x = (direction == -1) ? 40.0f + std::rand() % 20 : -40.0f - std::rand() % 20;

				enemy->SetPosition({ x, y, 30.0f });
				enemy->SetDirection(direction);
				enemy->SetSpeed(0.2f + static_cast<float>(std::rand()) / RAND_MAX * 0.5f);
			}
		}
	}
	// ========================================================================
	
	// プレイヤー更新
	player_->Update();

	worldTransform_.UpdateMatrix();
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


	// プレイヤーの描画
	player_->Draw(&camera_, playerTextureHandle_);

	// 敵の描画
	for (Enemy* enemy : enemies_) {
		enemy->Draw(&camera_, enemyTextureHandle_);
	}

	// 照準の描画
	crosshair_->Draw(&camera_);


	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

	// 3Dモデル描画前処理
	Model2::PreDraw(dxCommn->GetCommandList());


	// 3Dモデル描画後処理
	Model2::PostDraw();
}
