#include "Particle.h"
using namespace KamataEngine;

Particle::Particle() {}
Particle::~Particle() {}

void Particle::Initialize(Model* model, Camera* camera, const Vector3& startPos)
{
    model_ = model;
    camera_ = camera;

    worldTransform_.Initialize();
    worldTransform_.translation_ = startPos;

    // ランダムな速度
    velocity_ = {
        (rand() % 100 - 50) * 0.002f,
        -0.05f,
        (rand() % 100 - 50) * 0.002f
    };

    lifetime_ = 0.0f;
    alive_ = true;

    alpha = 1.0f - (lifetime_ / maxLifetime_);  
    if (alpha < 0.0f) {
        alpha = 0.0f;
    }
    color = {1.0f, 1.0f, 1.0f, 1.0f};
    objColor.Initialize();
    objColor.SetColor(color);
}

void Particle::Update()
{
    if (!alive_) {
        return;
    }

    worldTransform_.translation_.x += velocity_.x;
    worldTransform_.translation_.y += velocity_.y;
    worldTransform_.translation_.z += velocity_.z;

    velocity_.y -= 0.002f;

    lifetime_ += 0.016f;
    if (lifetime_ >= maxLifetime_) {
        alive_ = false;
    }

    objColor.SetColor(color);
    worldTransform_.UpdateMatrix();
    worldTransform_.TransferMatrix();
}

void Particle::Draw()
{
    if (!alive_) {
        return;
    }

    alpha = 1.0f - (lifetime_ / maxLifetime_);  
    if (alpha < 0.0f) {
        alpha = 0.0f;
    }

    color = {1.0f, 1.0f, 1.0f, alpha};
    objColor.SetColor(color); 

    float scaleFactor = alpha;
    worldTransform_.scale_ = { scaleFactor, scaleFactor, scaleFactor };

    model_->Draw(worldTransform_, *camera_,&objColor);
}
