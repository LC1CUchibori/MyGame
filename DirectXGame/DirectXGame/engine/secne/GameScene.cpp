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


	Model2::StaticFinalize();
}

void GameScene::Initialize() {
	// Model2の初期化
	Model2::StaticInitialize();

	// ランダム初期化
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	// プレイヤー
	playerTextureHandle_ = TextureManager::Load("BlackORE.png");
	playerModel_ = Model::Create();
	player_ = new Player();
	player_->Initialize(playerModel_, playerTextureHandle_, &camera_);

	enemyModel_ = Model::CreateFromOBJ("Enemy");
	enemy_ = new Enemy();
	enemy_->Initialize(enemyModel_, &camera_);

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

	// 「敵をPushで倒せ」スプライト
	pushPromptTextureHandle_ = TextureManager::Load("PushPrompt.png");
	pushPromptSprite_ = KamataEngine::Sprite::Create(pushPromptTextureHandle_, {400.0f, 150.0f});

	// エフェクト
	bossEffectTextures_[0] = TextureManager::Load("BlueEffect.png");
	bossEffectTextures_[1] = TextureManager::Load("GreenEffect.png");
	bossEffectTextures_[2] = TextureManager::Load("PurpleEffect.png");
	bossEffectTextures_[3] = TextureManager::Load("RedEffect.png");
	bossEffectTextures_[4] = TextureManager::Load("RainbowEffect.png");

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

	// =========================照準更新===============================
	// crosshair_->Update();

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
			enemy_->SetPosition({0.0f, 10.0f, 200.0f});
			enemy_->ResetApproach(); // ← 後述（Enemyに追加する関数）
		}
	}
	// ========================================================================

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

	if (enemy_ && player_) {
		for (auto& bullet : enemy_->GetBullets()) {
			if (!bullet->IsActive())
				continue;
			bullet->Update();
			Vector3 bulletPos = bullet->GetPosition();

			float dx = bulletPos.x - playerPos.x;
			float dy = bulletPos.y - playerPos.y;
			float dz = bulletPos.z - playerPos.z;
			float distance = sqrtf(dx * dx + dy * dy + dz * dz);

			float hitRadius = 2.0f;

			if (distance < hitRadius && !player_->IsDead()) {
				player_->Kill();
				bullet->SetInactive();
			}
		}
	}

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
					enemy_->OnDeath(); // 通常敵処理
					defeatCount_++;
				}
			}
		}
	}
	// ============================================================================

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

	// =====================================フェーズ処理======================================
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
			} else {
				// 失敗：赤まで
				for (int i = 0; i < 4; ++i) {
					if (per >= effectDisplayTimes_[i]) {
						effectIndex = i;
					}
				}
			}
		} else {
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
// ======================================================================================
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

	// 背景演出の描画
	/*for (BackEffect* backEffect : backEffects_) {
	    backEffect->Draw(&camera_, backEffectTextureHandle_);
	}*/

	// 照準の描画
	// crosshair_->Draw(&camera_);

	fade.Draw();

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

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
