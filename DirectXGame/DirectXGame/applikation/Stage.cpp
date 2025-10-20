#include "Stage.h"
using namespace KamataEngine;

Stage::Stage() {}

Stage::~Stage() {
    delete BGSprite_;
  ///*  delete LoopBGSprite1_;
  //  delete LoopBGSprite2_;*/
}

void Stage::Initialize() {
    // 元背景
    BGTextureHandle_ = TextureManager::Load("BG.png");
    BGSprite_ = KamataEngine::Sprite::Create(BGTextureHandle_, {0, 0});

    // ループ用背景2枚
    LoopBGTextureHandle_ = TextureManager::Load("LoopBG.png");
    LoopBGSprite1_ = KamataEngine::Sprite::Create(LoopBGTextureHandle_, {0, 0});
    LoopBGSprite2_ = KamataEngine::Sprite::Create(LoopBGTextureHandle_, {0, 0});

}

void Stage::Update() {
   
    if (BGSprite_) {
        // スプライトの位置を更新（Y方向にスクロール）
        Vector2 pos = { 0.0f, totalScroll };
        BGSprite_->SetPosition(pos);
    }

    if (LoopBGSprite1_) {
        Vector2 pos = { 0.0f,totalScroll + 1440 };
        LoopBGSprite1_->SetPosition(pos);
    }

    if (LoopBGSprite2_) {
        Vector2 pos = { 0.0f,totalScroll + 2160 };
        LoopBGSprite2_->SetPosition(pos);
    }

    if (totalScroll + 2160 > 0.0f) { // 背景下端が画面上端より上にある間だけスクロール
        totalScroll -= scrollSpeed;
    }
}

void Stage::Draw() {
    DirectXCommon* dxCommn = DirectXCommon::GetInstance();
    Sprite::PreDraw(dxCommn->GetCommandList());

    if (LoopBGSprite2_) LoopBGSprite2_->Draw();
    if (LoopBGSprite1_) LoopBGSprite1_->Draw(); // 下の背景から描画
    if (BGSprite_) BGSprite_->Draw();        

    Sprite::PostDraw();
}
