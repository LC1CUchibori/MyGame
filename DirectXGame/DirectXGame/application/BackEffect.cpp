#include "BackEffect.h"
#include <cassert>

BackEffect::BackEffect()
{
}

BackEffect::~BackEffect()
{
}

void BackEffect::Initialize(KamataEngine::Model* model, uint32_t textureHandle, KamataEngine::Camera* camera)
{
	// NULLポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;
	camera_ = camera;

	position_ = { 0.0f, -10.0f, 60.0f };

	worldTransform_.Initialize();

	worldTransform_.translation_ = position_;
	worldTransform_.UpdateMatrix();
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void BackEffect::Update()
{
	position_.x +=speed_*direction_;

	worldTransform_.translation_ = position_;
	worldTransform_.UpdateMatrix();
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void BackEffect::Draw(KamataEngine::Camera* camera, uint32_t textureHandle)
{
	model_->Draw(worldTransform_, *camera, textureHandle);
}
