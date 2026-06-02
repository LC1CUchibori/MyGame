#include "Stage.h"

Stage::Stage() {}

Stage::~Stage() {
    delete BGSprite_;
    delete LoopBGSprite1_;
    delete LoopBGSprite2_;
}

void Stage::Initialize() {
    // 固定背景
    BGTextureHandle_ = TextureManager::Load("BG.png");
    BGSprite_ = Sprite::Create(BGTextureHandle_, {0.0f, 0.0f});

    // ループ用背景
    LoopBGTextureHandle_ = TextureManager::Load("LoopBG.png");
    LoopBGSprite1_ = Sprite::Create(LoopBGTextureHandle_, {0.0f, 0.0f});
    LoopBGSprite2_ = Sprite::Create(LoopBGTextureHandle_, {0.0f, LoopBGHeight});

    scrollSpeed = 4.0f;

    // 初期配置
    if (BGSprite_) BGSprite_->SetPosition({0.0f, 0.0f});
    if (LoopBGSprite1_) LoopBGSprite1_->SetPosition({0.0f, 0.0f});
    if (LoopBGSprite2_) LoopBGSprite2_->SetPosition({0.0f, LoopBGHeight});
}

void Stage::Update() {
    // 固定BGのスクロール（消えるだけ）
    if (BGSprite_) {
        float y = BGSprite_->GetPosition().y - scrollSpeed;
        BGSprite_->SetPosition({0.0f, y});
    }

    // ループ用BG1
    if (LoopBGSprite1_) {
        float y1 = LoopBGSprite1_->GetPosition().y - scrollSpeed;
        if (y1 <= -LoopBGHeight) y1 = LoopBGHeight;
        LoopBGSprite1_->SetPosition({0.0f, y1});
    }

    // ループ用BG2
    if (LoopBGSprite2_) {
        float y2 = LoopBGSprite2_->GetPosition().y - scrollSpeed;
        if (y2 <= -LoopBGHeight) y2 = LoopBGHeight;
        LoopBGSprite2_->SetPosition({0.0f, y2});
    }
}

void Stage::Draw() {
    DirectXCommon* dxCommon = DirectXCommon::GetInstance();
    Sprite::PreDraw(dxCommon->GetCommandList());

    // 固定BGはスクロール中のみ描画
    if (BGSprite_ && BGSprite_->GetPosition().y + BGHeight > 0) {
        BGSprite_->Draw();
    }

    if (LoopBGSprite1_) LoopBGSprite1_->Draw();
    if (LoopBGSprite2_) LoopBGSprite2_->Draw();

    Sprite::PostDraw();
}
