#include "Player.h"
#include <cassert>
#include "PlayerBullet.h"
#include <algorithm>
#include "math/Vector3.h"
#include "math/Vector4.h"
#include "math/Matrix4x4.h"
#include "math/MathUtility.h"
#include "../engine/math/math.h"

using namespace KamataEngine::MathUtility;
using namespace KamataEngine;

Player::Player()
{
}

Player::~Player()
{
	// 弾をすべて解放
	for (auto* bullet : bullets_) {
		delete bullet;
	}
	bullets_.clear();

	// パーティクルをすべて解放
	for (auto* particle : particles_) {
		delete particle;
	}
	particles_.clear();
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

	targetPos_ = worldTransform_.translation_;

	worldTransform_.translation_ = position_;
	worldTransform_.UpdateMatrix();
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void Player::Update()
{
	// プレイヤーの移動処理
	PlayerMove();

	// 弾の処理
	BulletUpdate();

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
			if (!bullet->IsActive()) continue;
			bullet->Draw(camera);
		}
	}

	for (auto* particle_ : particles_) {
		particle_->Draw();
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
	// プレイヤーの向いている方向
	float fireAngle = worldTransform_.rotation_.z - 3.14f / 2.0f;
	Vector3 direction = {
		std::cos(fireAngle),
		-std::sin(fireAngle),
		0.0f
	};

	PlayerBullet* bullet = new PlayerBullet();
	bullet->Initialize(bulletModel_, camera_, worldTransform_.translation_, direction);
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
	/*ImGui::Begin("Player Debug");
	
	ImGui::End();*/
}

void Player::PlayerMove()
{
	Vector3 oldPos = worldTransform_.translation_;

	if (isDead_) return;

	// --- プレイヤー移動 ---
	if (input->PushKey(DIK_W)) targetPos_.y += speed_;
	if (input->PushKey(DIK_S)) targetPos_.y -= speed_;
	if (input->PushKey(DIK_A)) targetPos_.x -= speed_;
	if (input->PushKey(DIK_D)) targetPos_.x += speed_;

	worldTransform_.translation_.x =
		Lerp(worldTransform_.translation_.x, targetPos_.x, moveLerp_);
	worldTransform_.translation_.y =
		Lerp(worldTransform_.translation_.y, targetPos_.y, moveLerp_);


	// ========== パーティクル処理 =============

	// 追従パーティクル発生
	if (oldPos.x != worldTransform_.translation_.x ||
		oldPos.y != worldTransform_.translation_.y)
	{
		Particle* particle_ = new Particle();
		particle_->Initialize(particleModel_, camera_, worldTransform_.translation_);
		particles_.push_back(particle_);
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

	// =========================================


	// ============ プレイヤー追従 ===============
	// 
	// --- プレイヤーのスクリーン座標を取得 ---
	Vector2 playerScreen = WorldToScreen(worldTransform_.translation_, camera_);

	// --- マウス座標取得 ---
	GetCursorPos(&mousePos);
	ScreenToClient(GetActiveWindow(), &mousePos);

	// --- 差分 ---
	dx = mousePos.x - playerScreen.x;
	dy = mousePos.y - playerScreen.y;

	// --- 角度計算 ---
	angle = std::atan2(dy, dx);

	// --- Z回転に反映 ---
	worldTransform_.rotation_.z = angle + 3.14f / 2.0f;

	// ==================================================
}

void Player::BulletUpdate()
{
	// --- 弾発射 ---
	if (canShoot_ && input->TriggerKey(DIK_SPACE)) {
		Fire();
	}

	// --- 弾の更新 ---
	for (auto* bullet : bullets_) {
		bullet->Update();
	}

	// --- 弾を削除 ---
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
}

KamataEngine::Vector2 Player::WorldToScreen(const KamataEngine::Vector3& worldPos, KamataEngine::Camera* camera)
{
	// カメラのビュー行列と射影行列を取得
	const KamataEngine::Matrix4x4& view = camera->matView;
	const KamataEngine::Matrix4x4& proj = camera->matProjection;

	// ワールド座標をビュー座標に変換
	KamataEngine::Vector4 v4 = { worldPos.x, worldPos.y, worldPos.z, 1.0f };

	// ビュー行列 × ワールド座標
	KamataEngine::Vector4 viewPos;
	viewPos.x = view.m[0][0]*v4.x + view.m[1][0]*v4.y + view.m[2][0]*v4.z + view.m[3][0]*v4.w;
	viewPos.y = view.m[0][1]*v4.x + view.m[1][1]*v4.y + view.m[2][1]*v4.z + view.m[3][1]*v4.w;
	viewPos.z = view.m[0][2]*v4.x + view.m[1][2]*v4.y + view.m[2][2]*v4.z + view.m[3][2]*v4.w;
	viewPos.w = view.m[0][3]*v4.x + view.m[1][3]*v4.y + view.m[2][3]*v4.z + view.m[3][3]*v4.w;

	// 射影行列 × ビュー座標
	KamataEngine::Vector4 clip;
	clip.x = proj.m[0][0]*viewPos.x + proj.m[1][0]*viewPos.y + proj.m[2][0]*viewPos.z + proj.m[3][0]*viewPos.w;
	clip.y = proj.m[0][1]*viewPos.x + proj.m[1][1]*viewPos.y + proj.m[2][1]*viewPos.z + proj.m[3][1]*viewPos.w;
	clip.z = proj.m[0][2]*viewPos.x + proj.m[1][2]*viewPos.y + proj.m[2][2]*viewPos.z + proj.m[3][2]*viewPos.w;
	clip.w = proj.m[0][3]*viewPos.x + proj.m[1][3]*viewPos.y + proj.m[2][3]*viewPos.z + proj.m[3][3]*viewPos.w;

	// NDC に変換
	clip.x /= clip.w;
	clip.y /= clip.w;

	// スクリーン座標（仮に 1280x720）
	float screenX = (clip.x + 1.0f) * 0.5f * 1280.0f;
	float screenY = (1.0f - (clip.y + 1.0f) * 0.5f) * 720.0f;

	return { screenX, screenY };
}

KamataEngine::Vector3 Player::ScreenToWorld(int screenX, int screenY, float targetZ, KamataEngine::Camera* camera)
{
	// 画面サイズ（仮）
	float screenWidth = 1280.0f;
	float screenHeight = 720.0f;

	// NDCに変換
	float ndcX = (2.0f * screenX) / screenWidth - 1.0f;
	float ndcY = 1.0f - (2.0f * screenY) / screenHeight;

	// Near平面上の点（NDC z = 0）
	KamataEngine::Vector4 clipNear = { ndcX, ndcY, 0.0f, 1.0f };
	// Far平面上の点（NDC z = 1）
	KamataEngine::Vector4 clipFar = { ndcX, ndcY, 1.0f, 1.0f };

	// 射影行列の逆行列
	KamataEngine::Matrix4x4 invProj = KamataEngine::MathUtility::Inverse(camera->matProjection);
	// ビュー行列の逆行列
	KamataEngine::Matrix4x4 invView = KamataEngine::MathUtility::Inverse(camera->matView);

	// Near点をワールド座標に変換
	KamataEngine::Vector4 viewNear = Multiply(invProj, clipNear);
	viewNear.x /= viewNear.w;
	viewNear.y /= viewNear.w;
	viewNear.z /= viewNear.w;
	viewNear.w = 1.0f;
	KamataEngine::Vector4 worldNear = Multiply(invView, viewNear);

	// Far点をワールド座標に変換
	KamataEngine::Vector4 viewFar = Multiply(invProj, clipFar);
	viewFar.x /= viewFar.w;
	viewFar.y /= viewFar.w;
	viewFar.z /= viewFar.w;
	viewFar.w = 1.0f;
	KamataEngine::Vector4 worldFar = Multiply(invView, viewFar);

	// レイの方向を計算
	KamataEngine::Vector3 rayOrigin = { worldNear.x, worldNear.y, worldNear.z };
	KamataEngine::Vector3 rayDir = {
		worldFar.x - worldNear.x,
		worldFar.y - worldNear.y,
		worldFar.z - worldNear.z
	};

	// targetZの平面との交点を計算
	// rayOrigin + t * rayDir = targetZ (z成分)
	float t = (targetZ - rayOrigin.z) / rayDir.z;

	return {
		rayOrigin.x + t * rayDir.x,
		rayOrigin.y + t * rayDir.y,
		targetZ
	};
}
