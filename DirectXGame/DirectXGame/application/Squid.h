#pragma once

#include <KamataEngine.h>
#include "../engine/3d/Model2.h"
#include <vector>

class Squid
{
public:
	Squid();
	~Squid();

	void Initialize(KamataEngine::Model* model,KamataEngine::Camera* camera, const KamataEngine::Vector3& pos);

	void Update();

	void Draw(KamataEngine::Camera* camera);

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
};

