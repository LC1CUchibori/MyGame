#include "GameScene.h"

using namespace KamataEngine;

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete effectModel_;
	delete model_;
	delete player_;
	delete sharkTop_;
	delete stage;
	delete enemy_;

	// BackEffect 全部破棄
	for (BackEffect* e : backEffects_) {
		delete e;
	}
	backEffects_.clear();

	// Sprite 破棄
	delete gameOverSprite_;
	delete spawnSprite_;
	delete pushSprite_;

	// PushButton ゲージ
	delete RedGraph_;
	delete GreenGraph_;

	delete enemyHpBack_;
	delete enemyHpFront_;

	Model2::StaticFinalize();
}

void GameScene::Initialize() {
	// Model2の初期化
	Model2::StaticInitialize();

	// ランダム初期化
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	// プレイヤー
	playerModel_ = Model::CreateFromOBJ("Enemy");
	player_ = new Player();
	player_->Initialize(playerModel_, &camera_);

	enemyModel_ = Model::CreateFromOBJ("Enemy");
	enemy_ = new Enemy();
	enemy_->Initialize(enemyModel_, &camera_);
	enemy_->SetTarget(&player_->GetPosition());


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

		backEffect->SetPosition({x, y, 60.0f});
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

	// 鮫役物落下
	sharkTopModel_ = Model::CreateFromOBJ("sharkTop");
	sharkTop_ = new SharkTop();
	sharkTop_->Initialize(sharkTopModel_);

	gameOverTextureHandle_ = TextureManager::Load("GameOver.png");
	gameOverSprite_ = KamataEngine::Sprite::Create(gameOverTextureHandle_, {400.0f, 200.0f});

	// 出現スプライト初期化
	spawnTextureHandle_ = TextureManager::Load("spawn.png");
	spawnSprite_ = KamataEngine::Sprite::Create(spawnTextureHandle_, {spawnX_, 300.0f});

	isSpawnActive_ = true;
	spawnTimer_ = 0.0f;
	spawnX_ = -300.0f;

	fade.Initialize();

	// ボタンPUSHスプライト
	pushSpriteTextureHandle_ = TextureManager::Load("ButtonPush.png");
	pushSprite_ = KamataEngine::Sprite::Create(pushSpriteTextureHandle_, {640.0f, 360.0f}); // 画面中央

	// PushButtonバーの生成と初期化
	RedGraph_ = new Graph();
	RedGraph_->Initialize();
	//  グラフ生成
	RedGraph_->SetSize({300.0f,30.0f});
	RedGraph_->SetColor({ 1.0f, 0.0f, 0.0f, 0.5f });
	RedGraph_->SetPosition({ 500.0f,280.0f });
	GreenGraph_ = new Graph();
	GreenGraph_->Initialize();
	GreenGraph_->SetSize({300.0f,30.0f});
	GreenGraph_->SetColor({ 0.0f, 1.0f, 0.0f, 0.5f });
	GreenGraph_->SetPosition({ 500.0f,280.0f });

	// 敵HPバー（背景）
	enemyHpBack_ = new Graph();
	enemyHpBack_->Initialize();
	enemyHpBack_->SetSize({ 60.0f, 6.0f });
	enemyHpBack_->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });

	// 敵HPバー（前景）
	enemyHpFront_ = new Graph();
	enemyHpFront_->Initialize();
	enemyHpFront_->SetSize({ 60.0f, 6.0f });
	enemyHpFront_->SetColor({ 0.0f, 1.0f, 0.0f, 1.0f });

	// 「敵をPushで倒せ」スプライト
	pushPromptTextureHandle_ = TextureManager::Load("PushPrompt.png");
	pushPromptSprite_ = KamataEngine::Sprite::Create(pushPromptTextureHandle_, {400.0f, 150.0f});

	// エフェクト
	bossEffectTextures_[0] = TextureManager::Load("BlueEffect.png");
	bossEffectTextures_[1] = TextureManager::Load("GreenEffect.png");
	bossEffectTextures_[2] = TextureManager::Load("PurpleEffect.png");
	bossEffectTextures_[3] = TextureManager::Load("RedEffect.png");
	bossEffectTextures_[4] = TextureManager::Load("RainbowEffect.png");

	// 注意書き
	warningTextureHandle_ = TextureManager::Load("warning.png");
	warningSprite_ = Sprite::Create(warningTextureHandle_, { 400.0f,100.0f });

	for (int i = 0; i < 5; ++i) {
		bossEffectSprites_[i] = KamataEngine::Sprite::Create(bossEffectTextures_[i], {0.0f, 0.0f});
	}

	worldTransform_.Initialize();
	// カメラの初期化
	camera_.Initialize();
}

void GameScene::Update() {
	// 背景ステージ
	stage->Update();

	Vector3 crossPos = crosshair_->GetPosition();
	Vector3 playerPos = player_->GetPosition();

	// プレイヤー更新
	if (player_) {
		player_->Update();
	}

	// sharkTop更新
	if (player_ && player_->IsDead() && sharkTop_) {
		sharkTop_->Update();
	}

	// 敵の更新
	if (enemy_ && player_ && sharkTop_ && !(player_->IsDead() && sharkTop_->HasReturned())) {
		enemy_->Update();
	}

	// ゲームオーバーシーンに移行
	if (player_ && sharkTop_ && player_->IsDead() && sharkTop_->HasReturned()) {
		isGameOver_ = true;
	}

	if (enemy_) {
		enemy_->SetInactive();
	}

	// 当たり判定の処理
	IsCollision();

	// フェーズの処理
	UpdatePhase();

	// ===================== 出現スプライトのアニメーション =====================
	if (isSpawnActive_) {
		spawnTimer_ += 1.0f;

		// 出現演出
		float startX = -500.0f; // 左外
		float centerX = 390.0f; // 画面中央
		float endX = 1280.0f;   // 右外

		if (spawnTimer_ < 90) {
			// 左→中央へ移動
			float t = spawnTimer_ / 90.0f;
			spawnX_ = startX + (centerX - startX) * t;
		} else if (spawnTimer_ < 240) {
			// 中央で停止
			spawnX_ = centerX;
		} else if (spawnTimer_ < 360) {
			// 中央→右へ移動
			float t = (spawnTimer_ - 240.0f) / 120.0f;
			spawnX_ = centerX + (endX - centerX) * t;
		} else {
			// 終了
			isSpawnActive_ = false;
		}

		spawnSprite_->SetPosition({spawnX_, 300.0f});
	}
	// ==============================================================================

	
	// ===========================フェードの処理===================================
	if (enemy_ && enemy_->HasEscaped() && !isFadeActive_ && !isPhaseChanging_) {
		isFadeActive_ = true;
		isPhaseChanging_ = true;
		fade.StartFadeOut();
	}

	if (isFadeActive_) {
		fade.Update();

		if (fade.IsFadeOutEnd()) {
			phase_++;
			InitializePhase();
			fade.StartFadeIn();
		}

		if (fade.IsFadeInEnd()) {
			isFadeActive_ = false;
			isPhaseChanging_ = false;
		}
	}
	// ==============================================================================


	// ======================== チャレンジ中は敵を move 固定 ================================
	if (phase_ == 5 && bossState_ == BossChallengeState::Challenge && enemy_) {
		enemy_->SetForceMove(true);
	} else if (enemy_) {
		// チャレンジが終わったら元に戻す
		enemy_->SetForceMove(false);
	}
	// ======================================================================================

	
    // =========================== HPバー =================================
	if (enemy_ && !enemy_->IsDead()) {

		Vector3 enemyPos = enemy_->GetPosition();



		float barX = enemyPos.x * 20.0f + 640.0f;
		float barY = -enemyPos.y * 20.0f + 360.0f - 30.0f;

		float maxWidth = 60.0f;
		float height   = 6.0f;

		float hpRate =
			static_cast<float>(enemy_->GetHp()) /
			static_cast<float>(enemy_->GetMaxHp());

		if (hpRate < 0.0f) hpRate = 0.0f;
		if (hpRate > 1.0f) hpRate = 1.0f;

		float greenWidth = maxWidth * hpRate;

		// 赤（背景
		enemyHpBack_->SetSize({ maxWidth, height });
		enemyHpBack_->SetPosition({ barX, barY });

		// 緑（前
		enemyHpFront_->SetSize({ greenWidth, height });

		// 幅が減った分、中心を右にずらす
		float offsetX = (maxWidth - greenWidth) * 0.5f;
		enemyHpFront_->SetPosition({ barX - offsetX, barY });

 	}
    // ======================================================================
	
	worldTransform_.UpdateMatrix();
	worldTransform_.TransferMatrix();
}

void GameScene::Draw() {
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

	// ゲームオーバー表示
	if (isGameOver_ && gameOverSprite_) {
		gameOverSprite_->Draw();

		warningSprite_->Draw();
	}

	if (isPushPromptActive_ && pushPromptSprite_) {
		pushPromptSprite_->Draw();
	}

	if (bossState_ == BossChallengeState::Challenge && pushSprite_ && isPushSpriteVisible_) {
		pushSprite_->Draw();
		// バー
		RedGraph_->Draw();
		GreenGraph_->Draw();

		// エフェクト
		if (currentEffectIndex_ >= 0 && currentEffectIndex_ < 5) {
			bossEffectSprites_[currentEffectIndex_]->Draw();
		}
	}

	Sprite::PostDraw();

#pragma region 3Dオブジェクト描画
	dxCommn->ClearDepthBuffer();
	// 3Dオブジェクト描画前処理
	Model::PreDraw(dxCommn->GetCommandList());

	// プレイヤーの描画
	if (player_) {
		player_->Draw(&camera_, playerTextureHandle_);
	}

	if (enemy_ && player_ && !(player_->IsDead())) {
		// 敵の描画
		enemy_->Draw(&camera_);
	}

	if (sharkTop_) {
		sharkTop_->Draw(&camera_);
	}

	fade.Draw();

	// 3Dオブジェクト描画後処理
	Model::PostDraw();

#pragma endregion

	Sprite::PreDraw(dxCommn->GetCommandList());

	if (enemy_ && !enemy_->IsDead()) {
		enemyHpBack_->Draw();
		enemyHpFront_->Draw();
	}

	Sprite::PostDraw();

	// 3Dモデル描画前処理
	Model2::PreDraw(dxCommn->GetCommandList());

	// 3Dモデル描画後処理
	Model2::PostDraw();
}

void GameScene::InitializePhase() {
	switch (phase_) {
	case 1:
		enemy_->ResetForPhase({0.0f, 10.0f, 200.0f});
		break;
	case 2:
		enemy_->ResetForPhase({0.0f, 10.0f, 200.0f});
		break;
	case 3:
		enemy_->ResetForPhase({0.0f, 10.0f, 200.0f});
		break;
	case 4:
		enemy_->ResetForPhase({0.0f, 10.0f, 200.0f});
		break;
	case 5:
		enemy_->ResetForPhase({0.0f, 10.0f, 200.0f});
		break;
	default:
		break;
	}
}

void GameScene::UpdatePhase()
{
	switch (phase_) {
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		LastPhase();
		break;
	default:
		break;
	}
}

void GameScene::DrawImGui()
{
	player_->DrawImGui();
	ImGui::Text("Enemy HP : %d / %d",
		enemy_->GetHp(),
		enemy_->GetMaxHp());
}

void GameScene::IsCollision()
{
	// ================== プレイヤーと敵本体の当たり判定 ==================
	if (player_ && enemy_ && !player_->IsDead() && !enemy_->IsDead()) {
		Vector3 playerPos = player_->GetPosition();
		playerPos = player_->GetPosition();
		Vector3 enemyPos  = enemy_->GetPosition();

		float dx = playerPos.x - enemyPos.x;
		float dy = playerPos.y - enemyPos.y;
		float dz = playerPos.z - enemyPos.z;

		float dist = sqrtf(dx*dx + dy*dy + dz*dz);

		float hitRadius = 3.0f; // 当たり判定の大きさ（調整可）

		if (dist < hitRadius) {
			// プレイヤー死亡
			player_->Kill();
		}
	}
	// ====================================================================

	// =====================プレイヤーの弾と敵の当たり判定======================
	// プレイヤーの弾と敵の当たり判定
	if (player_ && enemy_) {
		for (auto* bullet : player_->GetBullets()) {
			if (!bullet->IsActive() || !enemy_)
				continue;

			Vector3 bulletPos = bullet->GetPosition();
			Vector3 enemyPos = enemy_->GetPosition();

			float dx = bulletPos.x - enemyPos.x;
			float dy = bulletPos.y - enemyPos.y;
			float dz = bulletPos.z - enemyPos.z;
			float dist = sqrtf(dx * dx + dy * dy + dz * dz);

			if (dist < 3.0f && !enemy_->IsDead()) {
				bullet->SetActive(false);
				if (phase_ == 5) {
					isBossHit_ = true;    // ボスヒットフラグ
					enemy_->StopMoving(); // 敵を停止させる新関数
					enemy_->SetCanShoot(false); 
					player_->SetCanShoot(false);

					// Push表示開始
					isPushPromptActive_ = true;
					pushPromptTimer_ = 0.0f;

					bossState_ = BossChallengeState::Start;
				} else {
					enemy_->TakeDamage(1); // 通常敵処理
					defeatCount_++;

				}
			}
		}
	}
	// ============================================================================
}

void GameScene::LastPhase()
{

	// ボスフェーズ攻撃ヒット後にStart状態にする
	if (bossState_ == BossChallengeState::Start) {
		bossPushStarted_ = false;
		bossPushTimer_ = 0.0f;
		isBossChallengeResult_ = false;

		GreenGraph_->SetSize({300.0f, 30.0f});
		bossState_ = BossChallengeState::Challenge;
	}

	if (bossState_ == BossChallengeState::Challenge) {

		// Spaceで成功確率だけ決める
		if (!isBossChallengeResult_ && input->TriggerKey(DIK_SPACE)) {
			int r = std::rand() % 100;
			isBossChallengeSuccess_ = (r < 51);
			isBossChallengeResult_ = true;
		}

		// 常にゲージは動く
		if (isPushSpriteVisible_) {
			bossPushTimer_++;
		}

		float per = bossPushTimer_ / bossPushDuration_;
		if (per > 1.0f) per = 1.0f;

		float maxWidth = 300.0f;
		float currentWidth = maxWidth * (1.0f - per);
		GreenGraph_->SetSize({ currentWidth, 30.0f });

		// 初期化
		effectDisplayTimes_[0] = 0.0f;  // 青
		effectDisplayTimes_[1] = 0.25f; // 緑
		effectDisplayTimes_[2] = 0.5f;  // 紫
		effectDisplayTimes_[3] = 0.75f; // 赤
		effectDisplayTimes_[4] = 0.9f;  // 虹

		// エフェクト
		int effectIndex = 0;
		if (isBossChallengeResult_) {
			if (isBossChallengeSuccess_) {
				// 成功：虹まで
				for (int i = 0; i < 5; ++i) {
					if (per >= effectDisplayTimes_[i]) {
						effectIndex = i;
					}
				}
				if (per >= 1.0f) effectIndex = 4;
			}
			else {
				// 失敗：赤まで
				for (int i = 0; i < 4; ++i) {
					if (per >= effectDisplayTimes_[i]) {
						effectIndex = i;
					}
				}
			}
		}
		else {
			// 赤まで
			for (int i = 0; i < 4; ++i) {
				if (per >= effectDisplayTimes_[i]) {
					effectIndex = i;
				}
			}
		}
		currentEffectIndex_ = effectIndex;


		// 時間切れ判定
		if (bossPushTimer_ >= bossPushDuration_) {
			if (isBossChallengeSuccess_) bossState_ = BossChallengeState::Success;
			else bossState_ = BossChallengeState::Failed;
		}

		// スプライト中央表示
		pushSprite_->SetPosition({400.0f, 150.0f});
	}

	// 成功時の処理
	if (bossState_ == BossChallengeState::Success) {
		// ここで敵を倒した扱いにする
		if (enemy_ && !enemy_->IsDead()) {
			enemy_->OnDeath(); // 敵死亡処理
			defeatCount_ = 5;  // 敵撃破カウント
		}
		if (enemy_ && enemy_->IsDead()) {
			isGameClear_ = true;
		}
	}

	// 失敗時の処理
	if (bossState_ == BossChallengeState::Failed) {
		if (player_ && !player_->IsDead()) {
			player_->Kill(); // プレイヤー死亡にする
		}

		if (player_ && sharkTop_ && player_->IsDead()) {
			sharkTop_->Update();

			if (sharkTop_->HasReturned()) {
				isGameOver_ = true;
			}
		}
	}

	if (isPushPromptActive_) {
		pushPromptTimer_ += 1.0f; // フレーム単位

		if (pushPromptTimer_ >= pushPromptDuration_) {
			// 1秒経過したら Pushスプライト表示
			isPushPromptActive_ = false;
			isPushSpriteVisible_ = true;
		}
	}
}
