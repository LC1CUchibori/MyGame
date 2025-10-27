#include "Player.h"
#include <cassert>
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

	position_ = { 00.0f, -15.0f, -10.0f };

	worldTransform_.Initialize();

	worldTransform_.translation_ = position_;
	worldTransform_.UpdateMatrix();
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void Player::Update()
{
	if (isDead_) return;

	if (input->PushKey(DIK_W))    worldTransform_.translation_.y += speed_;
	if (input->PushKey(DIK_S))  worldTransform_.translation_.y -= speed_;
	if (input->PushKey(DIK_A))  worldTransform_.translation_.x -= speed_;
	if (input->PushKey(DIK_D)) worldTransform_.translation_.x += speed_;

	//worldTransform_.translation_ = position_;
	worldTransform_.UpdateMatrix();
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void Player::Draw(KamataEngine::Camera* camera, uint32_t textureHandle)
{
	if (!isDead_) {
		model_->Draw(worldTransform_, *camera, textureHandle);
	}
}

void Player::SetYaw(float yaw)
{
	yaw_ = yaw;
	worldTransform_.rotation_ = { 0.0f,yaw_,0.0f };
	worldTransform_.UpdateMatrix();
	worldTransform_.TransferMatrix();
}
