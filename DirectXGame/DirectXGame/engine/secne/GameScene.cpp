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
	enemy_->Initialize(enemyModel_,&camera_);

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

	// 出現スプライト初期化
	spawnTextureHandle_ = TextureManager::Load("spawn.png");
	spawnSprite_ = KamataEngine::Sprite::Create(spawnTextureHandle_, { spawnX_, 300.0f });

	isSpawnActive_ = true;
	spawnTimer_ = 0.0f;
	spawnX_ = -300.0f;


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
	//crosshair_->Update();

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

	for (auto& bullet : enemy_->GetBullets()) {
		if (!bullet->IsActive()) continue;
		bullet->Update();
		Vector3 bulletPos = bullet->GetPosition();

		float dx = bulletPos.x - playerPos.x;
		float dy = bulletPos.y - playerPos.y;
		float dz = bulletPos.z - playerPos.z;
		float distance = sqrtf(dx*dx + dy*dy + dz*dz);

		float hitRadius = 1.0f;

		if (distance < hitRadius && !player_->IsDead()) {
			player_->Kill();
			bullet->SetInactive();
		}
	}


	// ===================== 出現スプライトのアニメーション =====================
	if (isSpawnActive_) {
		spawnTimer_ += 1.0f;

		// 出現演出
		float startX = -500.0f;  // 左外
		float centerX = 390.0f;  // 画面中央
		float endX = 1280.0f;    // 右外

		if (spawnTimer_ < 90) {
			// 左→中央へ移動
			float t = spawnTimer_ / 90.0f;
			spawnX_ = startX + (centerX - startX) * t;
		}
		else if (spawnTimer_ < 240) {
			// 中央で停止
			spawnX_ = centerX;
		}
		else if (spawnTimer_ < 360) {
			// 中央→右へ移動
			float t = (spawnTimer_ - 240.0f) / 120.0f;
			spawnX_ = centerX + (endX - centerX) * t;
		}
		else {
			// 終了
			isSpawnActive_ = false;
		}

		spawnSprite_->SetPosition({ spawnX_, 300.0f });
	}
    // ==============================================================================

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

	Sprite::PreDraw(dxCommn->GetCommandList());

	// 出現スプライト描画
	if (isSpawnActive_) {
		spawnSprite_->Draw();
	}

	Sprite::PostDraw();

#pragma region 3Dオブジェクト描画
	dxCommn->ClearDepthBuffer();
	// 3Dオブジェクト描画前処理
	Model::PreDraw(dxCommn->GetCommandList());


	// プレイヤーの描画
	player_->Draw(&camera_, playerTextureHandle_);

	// 敵の描画
	enemy_->Draw(&camera_);

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
