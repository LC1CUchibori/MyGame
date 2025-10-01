#include "TitleModel.h"

using namespace KamataEngine;

void TitleModel::Initialize(Model* model)
{
    // モデルをセット
    TitleModel_ = model;

    // ワールド変換を初期化
    worldTransform_.Initialize();


    // 初期位置を設定
    worldTransform_.translation_ = titlePosition_;
}

void TitleModel::Update()
{
    // 位置に速度を加算
    titlePosition_.x += titleVelocity_.x;

    // 境界に到達したら速度反転
    if (titlePosition_.x > rightLimit || titlePosition_.x < leftLimit)
    {
        titleVelocity_.x *= -1.0f;
    }

    // ワールド変換に反映
    worldTransform_.translation_ = titlePosition_;
    worldTransform_.UpdateMatrix();
}

void TitleModel::Draw(Camera* camera)
{
    TitleModel_->Draw(worldTransform_, *camera);
}
