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

	bool IsGameOver() const {
		return (player_ && sharkTop_ && player_->IsDead() && sharkTop_->HasReturned());
	}

	bool IsSceneReturnRequested() const { return isSceneReturnRequested_; }

	bool IsGameClear() const { return isGameClear_; }

	// ImGui描画
	void DrawImGui();

	// 当たり判定について
	void IsCollision();

	// フェーズ更新
	void UpdatePhase();

	// 最終ボス戦
	void LastPhase();

	void SquidInitialize();

private:
	// モデル
	KamataEngine::Model* model_ = nullptr;
	// ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;
	// インプット
	KamataEngine::Input* input = KamataEngine::Input::GetInstance();
	// カメラ
	KamataEngine::Camera camera_;
	// エフェクトモデル描画
	KamataEngine::Model2* effectModel_ = nullptr;

	uint32_t textureHandle_ = 0;

	// プレイヤー
	Player* player_ = nullptr;
	KamataEngine::Model* playerModel_ = nullptr;
	uint32_t playerTextureHandle_ = 0;

	// 敵
	Enemy* enemy_; // 複数の敵
	KamataEngine::Model* enemyModel_ = nullptr;
	uint32_t enemyTextureHandle_ = 0;

	// 背景演出
	std::vector<BackEffect*> backEffects_; 
	KamataEngine::Model* backEffectModel_ = nullptr;
	uint32_t backEffectTextureHandle_ = 0;
	Stage* stage = nullptr;

	// パーティクル
	std::vector<Particle*> particles_;

	// pushタイムバー
	Graph* RedGraph_ = nullptr;
	Graph* GreenGraph_ = nullptr;
	KamataEngine::Sprite* sprite_ = nullptr;
	bool isDamageActive_ = false; 
	const float maxHP = 100.0f;
	float hp_ = maxHP;

	// HPバー
	Graph* enemyHpBack_ = nullptr;
	Graph* enemyHpFront_ = nullptr;

	// ふぐ
	std::vector<TimeBomb*>bombs_;
	KamataEngine::Model* bombModel_ = nullptr;
	// ふぐの棘
	std::vector<TimeBomb*>insideBombs_;
	KamataEngine::Model* insideBombModel_ = nullptr;

	// イカモデル
	Squid* squid_ = nullptr;
	KamataEngine::Model* squidModel_ = nullptr;

	KamataEngine::Vector3 cursorPos_{0.0f, 0.0f, 30.0f}; // 初期位置
	float cursorSpeed_ = 0.5f; // 移動速度
	uint32_t crossTexture_;// 標準
	KamataEngine::Sprite* cursorSprite_ = nullptr;
	float cursorX_ = 400.0f; // 初期位置
	float cursorY_ = 300.0f;
	Crosshair3D* crosshair_ = nullptr;
	KamataEngine::Model* crosshairModel_ = nullptr;

	// サメの役物
	SharkTop* sharkTop_ = nullptr;
	KamataEngine::Model* sharkTopModel_ = nullptr;
	bool isSharkTopActive_ = false;

	bool isSceneReturnRequested_ = false; 
	Fade fade;

	// =============== スプライト類 ===========================

	// 出現スプライト用
	uint32_t spawnTextureHandle_ = 0;
	KamataEngine::Sprite* spawnSprite_ = nullptr;

	// 「敵をPushで倒せ」スプライト
	KamataEngine::Sprite* pushPromptSprite_ = nullptr;
	uint32_t pushPromptTextureHandle_;

	// 注意書きスプライト
	uint32_t warningTextureHandle_ = 0;
	KamataEngine::Sprite* warningSprite_ = nullptr;

	// ゲームオーバースプライト
	uint32_t gameOverTextureHandle_;
	KamataEngine::Sprite* gameOverSprite_ = nullptr;

	// エフェクトカラー
	uint32_t bossEffectTextures_[5];
	KamataEngine::Sprite* bossEffectSprites_[5];
	float effectDisplayTimes_[5] = {0.0f, 0.25f, 0.5f, 0.75f, 1.0f};

	// イカ墨のスプライト
	Sprite* inkSprite_ = nullptr;
	uint32_t inkTextureHandle_ = 0;

	bool isInkActive_ = false;
	float inkScale_ = 0.1f;  // 墨の大きさ
	float inkMaxScale_ = 1.5f;  // 墨のマックスサイズ
	float inkGrowSpeed_ = 0.02f;  // 吐くスピード
	const float inkBaseSize_ = 512.0f;  // 画像サイズ

	// ========================================================

	// 出現アニメーション制御用
	bool isSpawnActive_ = false;
	float spawnTimer_ = 0.0f;
	float spawnX_ = -300.0f; //

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

	KamataEngine::Sprite* pushSprite_ = nullptr;
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

	// Squid 出現用
	int squidTimer_ = 0;
	int squidAppearTime_ = 0;
	bool isSquidSpawned_ = false;
	Vector2 inkSpawnPos_;   // 吐く位置
	Vector2 inkTargetPos_;
};

