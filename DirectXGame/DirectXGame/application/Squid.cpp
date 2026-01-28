#include "Squid.h"
#include <cassert>

Squid::Squid()
{
}

Squid::~Squid()
{
}

void Squid::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& pos)
{
    assert(model);

    squidModel_ = model;
    camera_ = camera;

    worldTransform_.Initialize();
    worldTransform_.translation_ = pos;
    worldTransform_.UpdateMatrix();

    // GameScene側で出現タイミングを管理しているので、生成時点で即アクティブにする
    isActive_ = true;
    state_ = SquidState::Descend;
}

void Squid::Update()
{
    if (!isActive_) {
        return;
    }

    if (state_ == SquidState::Descend) {
        worldTransform_.translation_.y -= speed_;

        if (worldTransform_.translation_.y <= stopY_) {
            worldTransform_.translation_.y = stopY_;
            state_ = SquidState::stop;
        }
    }

    worldTransform_.UpdateMatrix();
}

void Squid::Draw(KamataEngine::Camera* camera)
{
    if (!squidModel_) return;
    if(!isActive_)return;


    squidModel_->Draw(worldTransform_, *camera);
}

void Squid::Reset(const KamataEngine::Vector3& startPos)
{
    // 位置を必ず上に戻す
    worldTransform_.translation_ = startPos;
    worldTransform_.UpdateMatrix();

    // 状態を「降下中」に戻す
    state_ = SquidState::Descend;

    // 落下速度も初期化
    speed_ = 0.2f;
}
