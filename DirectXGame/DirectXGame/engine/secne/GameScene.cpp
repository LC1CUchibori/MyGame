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

	enemyModel_ = Model::CreateFromOBJ("Enemy");
	enemy_ = new Enemy();
	enemy_->Initialize(enemyModel_, enemyTextureHandle_, &camera_);

	// ===============================背景演出の初期化================================
	backEffectTextureHandle_ = TextureManager::Load("white1x1.png");
	backEffectModel_ = Model::Create();

	for (int i = 0; i < 20; ++i) {
		BackEffect* backEffect = new BackEffect();
		backEffect->Initialize(playerModel_, playerTextureHandle_, &camera_);

		// 左から右だけ
		int direction = 1;

		// 高さランダム
		float y = -15.0f + static_cast<float>(std::rand()) / RAND_MAX * 50.0f;

		// 画面左端の少し外から出現
		float x = -50.0f - static_cast<float>(std::rand() % 20);

		backEffect->SetPosition({ x, y, 60.0f });
		backEffect->SetDirection(direction);
		backEffect->SetSpeed(0.2f + static_cast<float>(std::rand()) / RAND_MAX * 0.5f);

		backEffects_.push_back(backEffect);
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

	// ==========================背景演出の処理===========================
	//for (BackEffect* backEffect : backEffects_) {
	//	backEffect->Update();

	//	// 背景演出の位置を取得
	//	KamataEngine::Vector3 pos = backEffect->GetPosition();

	//	if (pos.x > 80.0f) {  // 80は画面右端目安
	//		float y = -15.0f + static_cast<float>(std::rand()) / RAND_MAX * 50.0f;
	//		float x = -80.0f - static_cast<float>(std::rand() % 20); // 左端外

	//		backEffect->SetPosition({ x, y, 60.0f });
	//		backEffect->SetDirection(1); // 左から右
	//		backEffect->SetSpeed(0.2f + static_cast<float>(std::rand()) / RAND_MAX * 0.5f);
	//	}
	//}
    // ===============================================================


	// =========================照準更新===============================
	crosshair_->Update();

	Vector3 crossPos = crosshair_->GetPosition();
	Vector3 playerPos = player_->GetPosition();

	if (input->TriggerKey(DIK_SPACE) && enemy_) {

		Vector3 enemyPos = enemy_->GetPosition();

		// クロスヘアと敵の距離
		float dx = enemyPos.x - crossPos.x;
		float dy = enemyPos.y - crossPos.y;
		float dz = enemyPos.z - crossPos.z;
		float dist = sqrtf(dx * dx + dy * dy + dz * dz);

		// 距離が一定以内なら命中
		if (dist < 3.0f) {
			// 命中処理：リスポーンさせる
			enemy_->SetPosition({ 0.0f, 10.0f, 200.0f });
			enemy_->ResetApproach(); // ← 後述（Enemyに追加する関数）
		}
	}
	// ========================================================================
	
	// プレイヤー更新
	player_->Update();

	// 敵の更新
	if (enemy_) {
		enemy_->Update();
	}

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
	enemy_->Draw(&camera_, enemyTextureHandle_);

	// 背景演出の描画
	/*for (BackEffect* backEffect : backEffects_) {
		backEffect->Draw(&camera_, backEffectTextureHandle_);
	}*/

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
