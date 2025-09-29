#include "Crosshair3D.h"

using namespace KamataEngine;

void Crosshair3D::Initialize(Model* model) {
    model_ = model;
    worldTransform_.Initialize();
    worldTransform_.translation_ = {0.0f, 0.0f, 30.0f}; // カメラ前方あたりに初期配置
    worldTransform_.UpdateMatrix();
    worldTransform_.TransferMatrix();
}

void Crosshair3D::Update() {
    Input* input = Input::GetInstance();

    if (input->PushKey(DIK_UP))    worldTransform_.translation_.y += speed_;
    if (input->PushKey(DIK_DOWN))  worldTransform_.translation_.y -= speed_;
    if (input->PushKey(DIK_LEFT))  worldTransform_.translation_.x -= speed_;
    if (input->PushKey(DIK_RIGHT)) worldTransform_.translation_.x += speed_;

    worldTransform_.UpdateMatrix();
    worldTransform_.TransferMatrix();
}

void Crosshair3D::Draw(Camera* camera) {
    model_->Draw(worldTransform_, *camera);
}
