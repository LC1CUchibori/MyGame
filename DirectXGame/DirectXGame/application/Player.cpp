#include "Player.h"
#include <cassert>
#include "PlayerBullet.h"
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

	// 弾モデル作成
	bulletModel_ = KamataEngine::Model::CreateFromOBJ("Enemy");

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

	// ===== 弾発射 =====
	if (input->TriggerKey(DIK_SPACE)) {
		Fire();
	}

	// 弾の更新
	for (auto* bullet : bullets_) {
		bullet->Update();
	}

	// 弾を削除
	bullets_.erase(
		std::remove_if(bullets_.begin(), bullets_.end(),
		[](PlayerBullet* b) {
			if (!b->IsActive()) {
				delete b;
				return true;
			}
			return false;
		}), 
		bullets_.end());


	//worldTransform_.translation_ = position_;
	worldTransform_.UpdateMatrix();
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void Player::Draw(KamataEngine::Camera* camera, uint32_t textureHandle)
{
	if (!isDead_) {
		model_->Draw(worldTransform_, *camera, textureHandle);

		for (auto* bullet : bullets_) {
			bullet->Draw(camera);
		}
	}
}

void Player::SetYaw(float yaw)
{
	yaw_ = yaw;
	worldTransform_.rotation_ = { 0.0f,yaw_,0.0f };
	worldTransform_.UpdateMatrix();
	worldTransform_.TransferMatrix();
}

void Player::Fire()
{
	PlayerBullet* bullet = new PlayerBullet();
	bullet->Initialize(bulletModel_, camera_, worldTransform_.translation_);
	bullets_.push_back(bullet);
}
