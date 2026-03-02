#include "GameScene.h"

using namespace KamataEngine;

GameScene::GameScene() {}

GameScene::~GameScene() {

	backEffects_.clear();

	bombs_.clear();

	Model2::StaticFinalize();
}

void GameScene::Initialize() {
	// Model2の初期化
	Model2::StaticInitialize();

	// ランダム初期化
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	// プレイヤー
	playerModel_.reset(Model::CreateFromOBJ("Enemy"));
	player_ = std::make_unique<Player>();
	player_->Initialize(playerModel_.get(), &camera_);

	enemyModel_.reset(Model::CreateFromOBJ("Enemy"));
	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize(enemyModel_.get(), &camera_);
	enemy_->SetTarget(&player_->GetPosition());


	// ===============================背景演出の初期化================================
	backEffectTextureHandle_ = TextureManager::Load("white1x1.png");
	backEffectModel_.reset(Model::Create());

	for (int i = 0; i < 20; ++i) {
		BackEffect* backEffect = new BackEffect();
		backEffect->Initialize(playerModel_.get(), playerTextureHandle_, &camera_);

		// 左から右だけ
		int direction = 1;

		// 高さランダム
		float y = -15.0f + static_cast<float>(std::rand()) / RAND_MAX * 50.0f;

		// 画面左端の少し外から出現
		float x = -50.0f - static_cast<float>(std::rand() % 20);

		backEffect->SetPosition({x, y, 60.0f});
		backEffect->SetDirection(direction);
		backEffect->SetSpeed(0.2f + static_cast<float>(std::rand()) / RAND_MAX * 0.5f);

		backEffects_.emplace_back(backEffect);
	}

	//===================================================================

	// ステージ
	stage = std::make_unique<Stage>();
	stage->Initialize();

	// 照準
	crosshairModel_ = Model::CreateFromOBJ("CrossHair");
	crosshair_ = new Crosshair3D();
	crosshair_->Initialize(crosshairModel_);

	// 鮫役物落下
	sharkTopModel_.reset(Model::CreateFromOBJ("sharkTop"));
	sharkTop_ =std::make_unique<SharkTop>();
	sharkTop_->Initialize(sharkTopModel_.get());

	gameOverTextureHandle_ = TextureManager::Load("GameOver.png");
	gameOverSprite_.reset(KamataEngine::Sprite::Create(gameOverTextureHandle_, {400.0f, 200.0f}));

	// 出現スプライト初期化
	spawnTextureHandle_ = TextureManager::Load("Danger.png");
	spawnSprite_.reset(KamataEngine::Sprite::Create(spawnTextureHandle_, {spawnX_, 300.0f}));

	isSpawnActive_ = true;
	spawnTimer_ = 0.0f;
	spawnX_ = -1000.0f;

	fade.Initialize();

	// ボタンPUSHスプライト
	pushSpriteTextureHandle_ = TextureManager::Load("ButtonPush.png");
	pushSprite_.reset(KamataEngine::Sprite::Create(pushSpriteTextureHandle_, {640.0f, 360.0f})); // 画面中央

	// PushButtonバーの生成と初期化
	RedGraph_ = std::make_unique<Graph>();
	RedGraph_->Initialize();
	//  グラフ生成
	RedGraph_->SetSize({300.0f,30.0f});
	RedGraph_->SetColor({ 1.0f, 0.0f, 0.0f, 0.5f });
	RedGraph_->SetPosition({ 500.0f,280.0f });
	GreenGraph_ = std::make_unique<Graph>();
	GreenGraph_->Initialize();
	GreenGraph_->SetSize({300.0f,30.0f});
	GreenGraph_->SetColor({ 0.0f, 1.0f, 0.0f, 0.5f });
	GreenGraph_->SetPosition({ 500.0f,280.0f });

	// 敵HPバー（背景）
	enemyHpBack_ = std::make_unique<Graph>();
	enemyHpBack_->Initialize();
	enemyHpBack_->SetSize({ 60.0f, 6.0f });
	enemyHpBack_->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });

	// 敵HPバー（前景）
	enemyHpFront_ = std::make_unique<Graph>();
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

	for (int i = 0; i < 5; ++i) {
		bossEffectSprites_[i] = KamataEngine::Sprite::Create(bossEffectTextures_[i], {0.0f, 0.0f});
	}

	// 注意書き
	warningTextureHandle_ = TextureManager::Load("warning.png");
	warningSprite_ = Sprite::Create(warningTextureHandle_, { 400.0f,100.0f });


	// 時限爆弾モデル
	bombModel_.reset(Model::CreateFromOBJ("Hugu"));
	insideBombModel_ = Model::CreateFromOBJ("InsideHugu");

	// イカモデル
	squidModel_ = Model::CreateFromOBJ("Squid");

	// ============= イカ出現の動き ===============
	squidTimer_ = 0;
	isSquidSpawned_ = false;

	// 5秒 + ランダム
	squidAppearTime_ = 300 + rand() % 180;
	// ============================================

	inkTextureHandle_ = TextureManager::Load("Ink.png");
	// Initialize
	inkSprite_ = Sprite::Create(inkTextureHandle_, { 640.0f,360.0f });
	inkSprite_->SetSize({ 0.1f, 0.1f });

	// ============= pauseスプライト =============
	pauseTextureHandle_ = TextureManager::Load("Pause.png");
	pauseSprite_.reset(Sprite::Create(
		pauseTextureHandle_,
		{ pauseStartX_, 300.0f }
	));

	pauseSpriteX_ = pauseStartX_;
	// =========================================

	// =================== ルール説明スプライト ===================
	ruleTextureHandle_ = TextureManager::Load("Rule.png");
	ruleSprite_ = Sprite::Create(ruleTextureHandle_, { 0.0f,0.0f });
	// ========================================================

	worldTransform_.Initialize();
	// カメラの初期化
	camera_.Initialize();
}

void GameScene::Update() {

	// ========== ポーズのトリガー =========
	if (input->TriggerKey(DIK_P)) {
		isPause_ = !isPause_;

		// ポーズON時は右外から出す
		if (isPause_) {
			pauseSpriteX_ = pauseStartX_;
		}
	}
	// ======================================

	// ====== ポーズ中 =======
	if (isPause_) {

		PauseUpdate();
		// ポーズ中は更新処理をスキップ
		return;
	}
	// =====================

	// 背景ステージ
	stage->Update();

	Vector3 crossPos = crosshair_->GetPosition();
	Vector3 playerPos = player_->GetPosition();


	if (player_ && enemy_) {

		// 突破チャレンジ中は撃てない
		if (phase_ == 5 &&
			(bossState_ == BossChallengeState::Start ||
				bossState_ == BossChallengeState::Challenge)) {

			player_->SetCanShoot(false);
		}
		else {
			// 通常時の射撃許可制御
			if (enemy_->IsApproachFinished()) {
				player_->SetCanShoot(true);
			}
			else {
				player_->SetCanShoot(false);
			}
		}
	}

	// プレイヤー更新
	if (player_) {
		player_->Update();
	}

	// sharkTop更新
	if (player_ && player_->IsDead() && sharkTop_) {
		sharkTop_->Update();
		for (TimeBomb* bomb : bombs_) {
			bomb->Kill();
		}
		bombs_.clear();
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

	// フェード中・フェーズ切り替え中はイカ処理しない
	if (!isFadeActive_ && !isPhaseChanging_&&phase_!=5) {
		SquidInitialize();
	}

	// 当たり判定の処理
	IsCollision();

	// フェーズの処理
	UpdatePhase();

	// ポーズ画面の処理
	GameSecneSpriteUpdate();

	// フェードの更新
	FadeUpdate();


	// ======================== チャレンジ中は敵を move 固定 ================================
	if (phase_ == 5 && bossState_ == BossChallengeState::Challenge && enemy_) {
		enemy_->SetForceMove(true);
	} else if (enemy_) {
		// チャレンジが終わったら元に戻す
		enemy_->SetForceMove(false);
	}
	// ======================================================================================

	

	if (enemy_->IsRequestBomb()) {
		TimeBomb* bomb = new TimeBomb();

		Vector3 pos = enemy_->GetPosition();
		pos.z = player_->GetPosition().z;

		bomb->Initialize(bombModel_.get(), insideBombModel_, &camera_, pos);
		bombs_.push_back(bomb);

		enemy_->ResetRequestBomb();
	}


	for (TimeBomb* bomb : bombs_) {
		bomb->Update();
	}

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

	// ----- プレイヤーの描画 ----
	if (player_) {
		player_->Draw(&camera_, playerTextureHandle_);
	}

	// ----- 敵の描画 ----
	if (enemy_ && player_ && !(player_->IsDead())) {
		enemy_->Draw(&camera_);
	}

	// ----- ふぐの描画 ----
	for (TimeBomb* bomb : bombs_) {
		bomb->Draw(&camera_);
	}

	// ----- サメの役物の描画 ----
	if (sharkTop_) {
		sharkTop_->Draw(&camera_);
	}

	// ----- イカの描画 -----
	if (phase_ != 5 && squid_) {
		squid_->Draw(&camera_);
	}

	fade.Draw();

	// 3Dオブジェクト描画後処理
	Model::PostDraw();

#pragma endregion

	Sprite::PreDraw(dxCommn->GetCommandList());

	// 出現スプライト描画
	if (isSpawnActive_) {
		spawnSprite_->Draw();
	}

	if (enemy_ && !enemy_->IsDead()&&player_&& !player_->IsDead()) {
		enemyHpBack_->Draw();
		enemyHpFront_->Draw();
	}

	// イカ墨のスプライト
	if (phase_ != 5 &&isInkActive_ && inkSprite_) {
		inkSprite_->Draw();
	}

	// ポーズスプライト
	if (isPause_) {
		pauseSprite_->Draw();
	}

	if (isRule_) {
		ruleSprite_->Draw(); // 表示
	}

	Sprite::PostDraw();

	// 3Dモデル描画前処理
	Model2::PreDraw(dxCommn->GetCommandList());

	// 3Dモデル描画後処理
	Model2::PostDraw();
}

void GameScene::InitializePhase() {
	if (player_) {
		player_->SetCanShoot(false);
	}

	// ふぐ初期化
	bombs_.clear();

	// ================= イカ出現管理リセット =================
	if (squid_) {
		squid_->Reset({ 0.0f, 50.0f, 50.0f });
	}
	
	squidPhaseTimer_ = 0;
	isSquidSpawned_ = false;
	squidAppearTime_ = 300 + rand() % 180;

	// ================= イカ墨リセット =================
	isInkActive_ = false;
	hasInkSpawned_ = false;
	inkScale_ = 0.1f;
	if (inkSprite_) {
		inkSprite_->SetSize({ 0.1f, 0.1f });
	}

	// ================= 敵の初期化 ======================
	if (enemy_) {
		enemy_->ResetDashTimer();
	}

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
		enemy_->SetMaxDashCount(1);
		break;
	case 2:
		enemy_->SetMaxDashCount(2);
		break;
	case 3:
		enemy_->SetMaxDashCount(3);
		break;
	case 4:
		enemy_->SetMaxDashCount(4);
		break;
	case 5:
		enemy_->SetMaxDashCount(5);
		LastPhase();
		break;
	default:
		break;
	}
}

void GameScene::DrawImGui()
{
	player_->DrawImGui();
	/*ImGui::Text("Enemy HP : %d / %d",
		enemy_->GetHp(),
		enemy_->GetMaxHp());*/
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
			squid_->Deactivate();
			isInkActive_ = false;
		}
	}
	// ====================================================================

	// ===================== プレイヤーの弾と敵の当たり判定 ======================
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

					if (defeatCount_ >= 10) {
						if (squid_) {
							squid_->Deactivate();
						}
						isInkActive_ = false;
					}
				}
			}
		}
	}
	// ============================================================================

	// =================== プレイヤーと爆弾の当たり判定 =======================
	if (player_ && !player_->IsDead()) {
		for (TimeBomb* bomb : bombs_) {

			Vector3 bombPos   = bomb->GetPosition();
			Vector3 playerPos = player_->GetPosition();

			float dx = bombPos.x - playerPos.x;
			float dy = bombPos.y - playerPos.y;
			float dz = bombPos.z - playerPos.z;

			float dist = std::sqrt(dx*dx + dy*dy + dz*dz);

			// =========================
			// ① 爆弾本体（爆発前でも即死）
			// =========================
			float bodyRadius = 2.0f * bomb->GetScale(); // 爆弾サイズ

			if (bomb->IsAlive() && dist <= bodyRadius) {
				player_->Kill();
				squid_->Deactivate();
				isInkActive_ = false;
				return;
			}

			// =========================
			// ② 爆発中の爆風
			// =========================
			if (bomb->IsAlive() &&bomb->IsExplode()) {
				float explosionRadius = 4.0f * bomb->GetScale(); // 爆風サイズ

				if (dist <= explosionRadius) {
					player_->Kill();
					squid_->Deactivate();
					isInkActive_ = false;
					return;
				}
			}
		}
	}
	// ===================================================================
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
			isBossChallengeSuccess_ = (r < 100);
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

void GameScene::SquidInitialize()
{
	// フェーズ開始からの経過時間
	squidPhaseTimer_++;

	// まだ出現待ち時間に達していなければ何もしない
	if (squidPhaseTimer_ < squidAppearTime_) {
		return;
	}

	if (!isSquidSpawned_ ) {

		squid_ = new Squid();
		squid_->Initialize(
			squidModel_,
			&camera_,
			{ 0.0f, 30.0f, 50.0f } // 上から出す
		);

		isSquidSpawned_ = true;
	}

	// イカの更新
	if (squid_) {
		squid_->Update();
	}

	if (squid_ && squid_->IsStopped() && !hasInkSpawned_) {
		isInkActive_ = true;
		hasInkSpawned_ = true;
		isInkFading_ = false;

		inkScale_ = 0.1f;
		inkAlpha_ = 1.0f;
		inkTimer_ = 0.0f;

		// ===== 吐き始め =====
		Vector3 squidWorld = squid_->GetPosition();
		inkSpawnPos_.x = squidWorld.x * 20.0f + 640.0f;
		inkSpawnPos_.y = -squidWorld.y * 20.0f + 360.0f;

		inkSprite_->SetPosition(inkSpawnPos_);

		// ===== 毎回ランダムな最終位置を生成 =====
		float maxSize = inkBaseSize_ * inkMaxScale_;
		float half = maxSize * 0.5f;

		// スプライトの中心を画面内に収める
		float minX = 0.0f;
		float maxX = 1280.0f - maxSize; // 左上基準なので右端は maxSize 分余裕を取る
		float minY = 0.0f;
		float maxY = 720.0f - maxSize;  // 下端も同様

		inkTargetPos_.x = minX + static_cast<float>(rand()) / RAND_MAX * (maxX - minX);
		inkTargetPos_.y = minY + static_cast<float>(rand()) / RAND_MAX * (maxY - minY);

		// 実際の描画位置にスプライト中心を合わせる
		inkSprite_->SetPosition({ inkTargetPos_.x + half, inkTargetPos_.y + half });

	}

	// 大きくなる
	if (isInkActive_) {
		// サイズ拡大
		inkScale_ += inkGrowSpeed_;
		if (inkScale_ > inkMaxScale_) {
			inkScale_ = inkMaxScale_;
		}

		// 進行度
		float t = inkScale_ / inkMaxScale_;
		if (t > 1.0f) t = 1.0f;

		// 吐き位置から最終位置へ補間
		Vector2 pos;
		pos.x = inkSpawnPos_.x + (inkTargetPos_.x - inkSpawnPos_.x) * t;
		pos.y = inkSpawnPos_.y + (inkTargetPos_.y - inkSpawnPos_.y) * t;

		inkSprite_->SetPosition(pos);

		float size = inkBaseSize_ * inkScale_;
		inkSprite_->SetSize({ size, size });
	}

	if (isInkActive_) {

		inkTimer_++;

		// 5秒経過したらフェード開始
		if (inkTimer_ >= inkStayTime_) {
			isInkFading_ = true;
		}

		// フェード処理
		if (isInkFading_) {
			float fadeT = (inkTimer_ - inkStayTime_) / inkFadeTime_;
			if (fadeT > 1.0f) fadeT = 1.0f;

			inkAlpha_ = 1.0f - fadeT;
			inkSprite_->SetColor({ 1.0f, 1.0f, 1.0f, inkAlpha_ });

			// 完全に消えたら終了
			if (fadeT >= 1.0f) {
				isInkActive_ = false;
				hasInkSpawned_ = false;
				inkSprite_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
			}
		}
	}

	if (phase_ == 5) {

		// イカ削除
		if (squid_) {
			delete squid_;
			squid_ = nullptr;
		}

		// 墨リセット
		isInkActive_ = false;
		hasInkSpawned_ = false;

		if (inkSprite_) {
			inkSprite_->SetColor({1,1,1,1});
			inkSprite_->SetSize({0.1f,0.1f});
		}
	}
}

void GameScene::GameSecneSpriteUpdate()
{

	// ===================== 出現スプライトのアニメーション制御 =====================
	if (isSpawnActive_) {
		spawnTimer_ += 1.0f;

		// 出現演出
		float startX = -1020.0f; // 左外
		float centerX = 150.0f; // 画面中央
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
}

void GameScene::PauseUpdate()
{
	// ===================== ポーズ画面のスプライト制御 ======================

	if (isPause_) {
		// 右からスライドイン
		if (pauseSpriteX_ > pauseTargetX_) {
			pauseSpriteX_ -= pauseSpeed_;
			if (pauseSpriteX_ < pauseTargetX_) {
				pauseSpriteX_ = pauseTargetX_;
			}
		}

		// 1キーで再開
		if (isPause_) {
			if (input->TriggerKey(DIK_1)) {
				isPause_ = false;
			}
		}

		// 2キーで操作ルール表示切替
		if (isPause_) {
			if (input->TriggerKey(DIK_2)) {
				isRule_ = !isRule_; 
			}
		}

		// 3キーでタイトルに戻る
		if (isPause_) {
			if (input->TriggerKey(DIK_3)) {
				isReturnToTitle_ = true;
			}
		}
		pauseSprite_->SetPosition({ pauseSpriteX_, 130.0f });
	}
	// ============================================================================
}

void GameScene::FadeUpdate()
{
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
}
