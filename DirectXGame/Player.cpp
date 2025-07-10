#include "Player.h"
using namespace KamataEngine;

Player::Player()
{
}

Player::~Player()
{
}

void Player::Initialize(KamataEngine::Model* model,uint32_t textureHandle,KamataEngine::Camera*camera)
{
	// NULLポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;
	camera_ = camera;

	input = Input::GetInstance(); 

	position_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.Initialize();
}

void Player::Update()
{
	// プレイヤー移動入力処理
	const float speed = 0.2f;

	if (input->PushKey(DIK_A)) {
		position_.x -= speed;
	}
	if (input->PushKey(DIK_D)) {
		position_.x += speed;
	}
	if (input->PushKey(DIK_W)) {
		position_.y += speed;
	}
	if (input->PushKey(DIK_S)) {
		position_.y -= speed;
	}

	worldTransform_.translation_ = position_;
	worldTransform_.UpdateMatrix();
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void Player::Draw(KamataEngine::Camera* camera, uint32_t textureHandle)
{
	model_->Draw(worldTransform_, *camera, textureHandle);
}
