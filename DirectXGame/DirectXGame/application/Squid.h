#pragma once

#include <KamataEngine.h>
#include "../engine/3d/Model2.h"
#include <vector>

class Squid {
public:
	Squid();
	~Squid();

	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& pos);

	void Update();

	void Draw(KamataEngine::Camera* camera);

	void Reset(const KamataEngine::Vector3& startPos);

	void Deactivate();



	bool IsActive() const { return isActive_; }


	// 止まったかどうか
	bool IsStopped() const { return state_ == SquidState::stop; }

	// 座標取得Getter
	const KamataEngine::Vector3& GetPosition() const { return worldTransform_.translation_; }

private:
	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;
	// モデル
	KamataEngine::Model* squidModel_;
	// カメラ
	KamataEngine::Camera* camera_ = nullptr;

	enum SquidState
	{
		Descend, // 上から落ちる
		stop     // 停止
	};

	SquidState state_ = SquidState::Descend;
	float speed_ = 0.3f;
	float stopY_ = 5.0f;

	// 出現制御
	bool isActive_ = false;      // 出現しているか

	// =================== イカ関連 =========================
	// Squid 出現用
	int squidTimer_ = 0;
	int squidAppearTime_ = 0;
	bool isSquidSpawned_ = false;
	KamataEngine::Vector2 inkSpawnPos_;   // 吐く位置
	KamataEngine::Vector2 inkTargetPos_;       // 吐く目標位置
	bool hasInkSpawned_ = false; // 墨のスプライトフラグ
	int squidPhaseTimer_ = 0;

	// イカ墨フェード用
	float inkTimer_ = 0.0f;
	const float inkStayTime_ = 300.0f;   // 5秒
	const float inkFadeTime_ = 120.0f;   // フェード時間（2秒）
	float inkAlpha_ = 1.0f;
	bool isInkFading_ = false;

	bool isInkActive_ = false;
	float inkScale_ = 0.1f;  // 墨の大きさ
	float inkMaxScale_ = 1.5f;  // 墨のマックスサイズ
	float inkGrowSpeed_ = 0.02f;  // 吐くスピード
	const float inkBaseSize_ = 512.0f;  // 画像サイズ
	// =======================================================
};

