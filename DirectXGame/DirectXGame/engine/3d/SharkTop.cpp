#include "SharkTop.h"
#include "imgui_impl_win32.h"

void SharkTop::Initialize(KamataEngine::Model* model) {
	sharkTopModel_ = model;
	worldTransform_.Initialize();

	// 初期位置は画面上に設定しておく
	worldTransform_.translation_ = {45, -30.0f, 0.0f};
	initialPosition_ = worldTransform_.translation_; // 元の位置を記録

	// タイマー初期化
	rotationTimer_ = 0.0f;
	isRotationStopped_ = false;
	isReturning_ = false;
	hasReturned_ = false;

	worldTransform_.UpdateMatrix();
	worldTransform_.TransferMatrix();
}

void SharkTop::Update() {
	// すでに戻り終わったら動かさない
	if (hasReturned_) return;

	const float deltaTime = 1.0f / 60.0f;
	rotationTimer_ += deltaTime;

	if (!isRotationStopped_ && !isReturning_) {
		worldTransform_.rotation_.z -= 0.05f;
		if (rotationTimer_ >= 0.55f) {
			isRotationStopped_ = true;
			rotationTimer_ = 0.0f;
		}
	}
	else if (isRotationStopped_ && !isReturning_) {
		if (rotationTimer_ >= 3.0f) {
			isRotationStopped_ = false;
			isReturning_ = true; // 戻るフェーズへ
			rotationTimer_ = 0.0f;
		}
	}
	else if (isReturning_) {
		worldTransform_.rotation_.z += 0.05f; // 逆回転

		// 徐々に初期位置に戻る
		KamataEngine::Vector3& pos = worldTransform_.translation_;
		pos.x += (initialPosition_.x - pos.x) * 0.1f;
		pos.y += (initialPosition_.y - pos.y) * 0.1f;
		pos.z += (initialPosition_.z - pos.z) * 0.1f;

		if (rotationTimer_ >= 0.6f) {
			worldTransform_.translation_ = initialPosition_;
			worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
			isReturning_ = false;
			hasReturned_ = true; // 完全停止
		}
	}

	worldTransform_.UpdateMatrix();
	worldTransform_.TransferMatrix();
}

void SharkTop::Draw(KamataEngine::Camera* camera) {
	if (!sharkTopModel_) return;
	sharkTopModel_->Draw(worldTransform_, *camera);
}
