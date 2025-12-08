#include "Player.h"
#include <cassert>
#include "PlayerBullet.h"
#include <algorithm>
using namespace KamataEngine;

Player::Player()
{
}

Player::~Player()
{
}

void Player::Initialize(KamataEngine::Model* model,KamataEngine::Camera*camera)
{
	// NULLポインタチェック
	assert(model);

	model_ = model;
	camera_ = camera;
	input = Input::GetInstance(); 

	position_ = { 00.0f, -15.0f, -10.0f };
	worldTransform_.Initialize();

	// 弾モデル作成
	bulletModel_ = KamataEngine::Model::CreateFromOBJ("EnemyBullet");

	// パーティクル
	particleModel_ = Model::CreateFromOBJ("ParticleBall");

    mousePosX = 0.0f;
	mousePosY = 0.0f;

	worldTransform_.rotation_.x = 3.14f / 1.0f;

	worldTransform_.translation_ = position_;
	worldTransform_.UpdateMatrix();
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void Player::Update()
{
	Vector3 oldPos = worldTransform_.translation_;

	if (isDead_) return;

	// --- プレイヤー移動 ---
	if (input->PushKey(DIK_W))    worldTransform_.translation_.y += speed_;
	if (input->PushKey(DIK_S))  worldTransform_.translation_.y -= speed_;
	if (input->PushKey(DIK_A))  worldTransform_.translation_.x -= speed_;
	if (input->PushKey(DIK_D)) worldTransform_.translation_.x += speed_;

	// WASDで移動後に追従パーティクル発生
	if (oldPos.x != worldTransform_.translation_.x ||
		oldPos.y != worldTransform_.translation_.y)
	{
		Particle* p = new Particle();
		p->Initialize(particleModel_, camera_, worldTransform_.translation_);
		particles_.push_back(p);
	}

	for (auto* p : particles_) {
		p->Update();
	}

	particles_.erase(
		std::remove_if(particles_.begin(), particles_.end(),
			[](Particle* p) {
				if (!p->IsAlive()) {
					delete p;
					return true;
				}
				return false;
			}),
		particles_.end());


	// =====================
	// プレイヤーの向きをマウスへ
	// =====================
	
	// マウスのスクリーン位置を取得
	GetCursorPos(&mousePos);
	ScreenToClient(GetActiveWindow(), &mousePos);

	mouse.x = (float)mousePos.x;
	mouse.y = (float)mousePos.y;

	//// 仮想スクリーン上のプレイヤー位置（画面中央に固定）
	//float playerScreenX = 640.0f;
	//float playerScreenY = 360.0f;

    dx = mousePos.x - worldTransform_.translation_.x;
    dy = mousePos.y - worldTransform_.translation_.y;
    angle = std::atan2(dy, dx);
	worldTransform_.rotation_.z = (angle - 3.14f * 1.5f) * (-1.0f);

	// マウス方向ベクトル
	//dx = static_cast<float>(mousePos.x) - worldTransform_.translation_.x;
	//dy = static_cast<float>(mousePos.y) - worldTransform_.translation_.y;

	//// プレイヤーの頭が常にマウス方向を向く
	//yaww = std::atan2(dx, -dy);
	//worldTransform_.rotation_.z = yaww;

	

	// ===== 弾発射 =====
	if (canShoot_ && input->TriggerKey(DIK_SPACE)) {
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

	for (auto* p : particles_) {
		p->Draw();
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

// 死亡処理
void Player::Kill() {
	isDead_ = true;

	for (auto& particle : particles_) {
		particle->Kill();
	}
}

void Player::DrawImGui()
{
	ImGui::Begin("Player Debug");
	float r = angle * (180.0f / float(3.14f));
	ImGui::DragFloat2("Mouse Cursol",&mouse.x,0.0f);
	ImGui::DragFloat2("Player Position", &worldTransform_.translation_.x, 0.0f);
	ImGui::DragFloat("Player Yaw: %.3f rad (%.1f deg)", &r);
	ImGui::Text("Player Position: (%.1f, %.1f, %.1f)", 
		worldTransform_.translation_.x, 
		worldTransform_.translation_.y, 
		worldTransform_.translation_.z);
	ImGui::End();
}
