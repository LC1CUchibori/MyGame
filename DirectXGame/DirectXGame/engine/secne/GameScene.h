#pragma once
#include "KamataEngine.h"
#include "../3d/Model2.h"
#include "../../application/Stage.h"
#include "../../application/Player.h"
#include "../../application/Enemy.h"
#include "../../application/BackEffect.h"
#include "../../application/Particle.h"
#include <cstdlib>
#include <ctime>
#include "../3d/CrossHair3D.h"
#include "../3d/SharkTop.h"
#include "DirectXGame/engine/2d/Fade.h" 
#include <DirectXGame/application/Graph.h>
#include "../../application/TimeBomb.h"
#include "../../application/Squid.h"
#include <list>

class GameScene
{
public:
	GameScene();

	~GameScene();

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

	// フェーズ初期化
	void InitializePhase();

	// ImGui描画
	void DrawImGui();

	// 当たり判定について
	void IsCollision();

	// フェーズ更新
	void UpdatePhase();

	// 最終ボス戦
	void LastPhase();

	// イカの初期化
	void SquidInitialize();

	// ゲーム内のスプライト更新
	void GameSecneSpriteUpdate();

	void PauseUpdate();

	void FadeUpdate();

	void ResetReturnFlag() { isReturnToTitle_ = false; }


	// =================== 取得用 ========================
	bool IsGameOver() const {
		return (player_ && sharkTop_ && player_->IsDead() && sharkTop_->HasReturned());
	}

	bool IsSceneReturnRequested() const { 
		return isSceneReturnRequested_; 
	}

	bool IsGameClear() const {
		return isGameClear_;
	}

	bool IsReturnToTitle() const {
		return isReturnToTitle_; 
	}

	//  =================================================

private:
	// モデル
	std::unique_ptr<KamataEngine::Model> model_;
	// ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;
	// インプット
	KamataEngine::Input* input = KamataEngine::Input::GetInstance();
	// カメラ
	KamataEngine::Camera camera_;
	// エフェクトモデル描画
	std::unique_ptr<KamataEngine::Model2> effectModel_;

	uint32_t textureHandle_ = 0;

	// プレイヤー
	std::unique_ptr<Player> player_;
	std::unique_ptr<KamataEngine::Model> playerModel_;
	uint32_t playerTextureHandle_ = 0;

	// 敵
	std::unique_ptr<Enemy> enemy_; // 複数の敵
	std::unique_ptr<KamataEngine::Model> enemyModel_;
	uint32_t enemyTextureHandle_ = 0;

	// 背景演出
	std::vector<std::unique_ptr<BackEffect>> backEffects_; 
	std::unique_ptr<KamataEngine::Model> backEffectModel_;
	uint32_t backEffectTextureHandle_ = 0;
	std::unique_ptr<Stage> stage;

	// パーティクル
	std::vector<std::unique_ptr<Particle>> particles_;
	Model* particleModel_ = nullptr;

	// pushタイムバー
	std::unique_ptr<Graph> RedGraph_;
	std::unique_ptr<Graph> GreenGraph_;
	KamataEngine::Sprite* sprite_ = nullptr;
	bool isDamageActive_ = false; 
	const float maxHP = 100.0f;
	float hp_ = maxHP;

	// HPバー
	std::unique_ptr<Graph> enemyHpBack_ ;
	std::unique_ptr<Graph> enemyHpFront_;

	// ふぐ
	std::vector<TimeBomb*>bombs_;
	std::unique_ptr<KamataEngine::Model> bombModel_;
	// ふぐの棘
	std::vector<TimeBomb*>insideBombs_;
	KamataEngine::Model* insideBombModel_ = nullptr;

	// イカモデル
	Squid* squid_ = nullptr;
	KamataEngine::Model* squidModel_ = nullptr;

	uint32_t crossTexture_;
	KamataEngine::Sprite* cursorSprite_ = nullptr;
	float cursorX_ = 400.0f;
	float cursorY_ = 300.0f;

	// サメの役物
	std::unique_ptr<SharkTop> sharkTop_;
	std::unique_ptr<KamataEngine::Model> sharkTopModel_;
	bool isSharkTopActive_ = false;

	bool isSceneReturnRequested_ = false; 
	Fade fade;

	// =============== スプライト類 ===========================

	// 出現スプライト用
	uint32_t spawnTextureHandle_ = 0;
	std::unique_ptr<KamataEngine::Sprite> spawnSprite_;

	// 「敵をPushで倒せ」スプライト
	KamataEngine::Sprite* pushPromptSprite_ = nullptr;
	uint32_t pushPromptTextureHandle_;

	// 注意書きスプライト
	uint32_t warningTextureHandle_ = 0;
	KamataEngine::Sprite* warningSprite_ = nullptr;

	// ゲームオーバースプライト
	uint32_t gameOverTextureHandle_;
	std::unique_ptr<KamataEngine::Sprite> gameOverSprite_;

	// エフェクトカラー
	uint32_t bossEffectTextures_[5];
	KamataEngine::Sprite* bossEffectSprites_[5];
	float effectDisplayTimes_[5] = {0.0f, 0.25f, 0.5f, 0.75f, 1.0f};

	// イカ墨のスプライト
	Sprite* inkSprite_ = nullptr;
	uint32_t inkTextureHandle_ = 0;

	// ルール説明
	KamataEngine::Sprite* ruleSprite_ = nullptr;
	uint32_t ruleTextureHandle_ = 0;
	bool isRule_ = false;

	bool isInkActive_ = false;
	float inkScale_ = 0.1f;  // 墨の大きさ
	float inkMaxScale_ = 1.5f;  // 墨のマックスサイズ
	float inkGrowSpeed_ = 0.02f;  // 吐くスピード
	const float inkBaseSize_ = 512.0f;  // 画像サイズ

	// ポーズ画面スプライト
	std::unique_ptr<KamataEngine::Sprite> pauseSprite_;
	uint32_t pauseTextureHandle_;

	// ポーズ指示スプライト
	std::unique_ptr<KamataEngine::Sprite> pauseOrderSprite_;
	uint32_t pauseOrderTextureHandle_;

	// ========================================================

	// 出現アニメーション制御用
	bool isSpawnActive_ = false;
	float spawnTimer_ = 0.0f;
	float spawnX_ = -1000.0f; 

	// ゲームオーバーフラグ
	bool isGameOver_ = false;

	// フェーズ
	int phaseCount_ = 1;
	int phase_ = phaseCount_;
	bool isPhaseChanging_ = false;
	bool isFadeActive_ = false; // フェード開始フラグ
	float phaseChangeTimer_ = 0.0f;

	// 敵のフェーズ回数
	int defeatCount_ = 0;
	bool isGameClear_ = false;

	// 突破チャレンジタイマー
	bool isBossHit_ = false;
	float challengeGauge_ = 0.0f;
	float challengeTimer_ = 0.0f;

	std::unique_ptr<KamataEngine::Sprite> pushSprite_;
	int pushSpriteTextureHandle_ = 0;

	bool isBossChallengeResultDecided_ = false; // 成功・失敗判定済みか

	enum class BossChallengeState { None, Start, Challenge, Success, Failed };

	// pushButton
	BossChallengeState bossState_ = BossChallengeState::None;
	bool isBossChallengeResult_ = false;   // 成功判定済みか
	bool isBossChallengeSuccess_ = false;  // 成功か失敗か
	bool bossPushStarted_ = false;         // ボタン押し開始
	float bossPushTimer_ = 0.0f;           // 5秒間カウント
	const float bossPushDuration_ = 300.0f; // 5秒 = 60fps x 5秒

	// タイマー
	float pushPromptTimer_ = 0.0f;
	float pushPromptDuration_ = 60.0f; // 60フレーム

	// 表示状態
	bool isPushPromptActive_ = false;
	bool isPushSpriteVisible_ = false; 
	int currentEffectIndex_ = 0;

	// =================== イカ関連 =========================
	// Squid 出現用
	int squidTimer_ = 0;
	int squidAppearTime_ = 0;
	bool isSquidSpawned_ = false;
	Vector2 inkSpawnPos_;   // 吐く位置
	Vector2 inkTargetPos_;       // 吐く目標位置
	bool hasInkSpawned_ = false; // 墨のスプライトフラグ
	int squidPhaseTimer_ = 0;

	// イカ墨フェード用
	float inkTimer_ = 0.0f;
	const float inkStayTime_ = 300.0f;   // 5秒
	const float inkFadeTime_ = 120.0f;   // フェード時間（2秒）
	float inkAlpha_ = 1.0f;
	bool isInkFading_ = false;
	// =======================================================

	// =============== ポーズ関連 ========================
	bool isPause_ = false;

	float pauseSpriteX_ = 0.0f;
	float pauseTargetX_ = 800.0f;   // 表示位置（右寄り）
	float pauseStartX_ = 1300.0f;   // 画面右外
	float pauseSpeed_ = 20.0f;      // スライド速度
	bool isReturnToTitle_ = false;  // タイトルに戻るフラグ
	// ====================================================

	bool isGameStopped_ = false;

	Vector3 cameraOffset_ = { 0.0f, 20.0f, -80.0f };
	float cameraFollowSpeed_ = 0.08f; // 小さいほどゆっくり追従

	float InkRespawnTimer_ = 0.0f;
	float InkRespawnTime_ = 0.0f;
	bool isInkRespawning_ = false;
};

