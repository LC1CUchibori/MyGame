#include "TitleModel.h"

using namespace KamataEngine;

void TitleModel::Initialize(Model* model)
{
    // モデルをセット
    TitleModel_ = model;

    // ワールド変換を初期化
    worldTransform_.Initialize();

    // 初期位置と回転を設定
    worldTransform_.translation_ = titlePosition_;
    worldTransform_.rotation_ = { 0.0f, 0.0f, 0.0f };

    // 回転速度（任意で調整可）
    rotationSpeed_ = { 0.0f, 0.02f, 0.0f }; // Y軸中心に回転
}

void TitleModel::Update()
{
    // 位置に速度を加算
    titlePosition_.x += titleVelocity_.x;

    // 境界に到達したら速度反転
    if (titlePosition_.x > rightLimit || titlePosition_.x < leftLimit)
    {
        titleVelocity_.x *= -1.0f;

        // 反転時に軽く回転方向も変化させると自然
        rotationSpeed_.y *= -1.0f;
    }

    // 回転を更新
    worldTransform_.rotation_.x += rotationSpeed_.x;
    worldTransform_.rotation_.y += rotationSpeed_.y;
    worldTransform_.rotation_.z += rotationSpeed_.z;

    // ワールド変換に反映
    worldTransform_.translation_ = titlePosition_;
    worldTransform_.UpdateMatrix();
}

void TitleModel::Draw(Camera* camera)
{
    TitleModel_->Draw(worldTransform_, *camera);
}
