#include "Stage.h"
#include <cmath>

using namespace KamataEngine;

Stage::Stage() {}

Stage::~Stage() {
	delete BGSprite_;
}

void Stage::Initialize() {
	BGTextureHandle_ = TextureManager::Load("BG.png");
	BGSprite_ = KamataEngine::Sprite::Create(BGTextureHandle_, { 0, 0 });
}

void Stage::Update() { 
        if (totalScroll > -720.0f) {
            totalScroll -= scrollSpeed;
            if (totalScroll < -720.0f) {
                totalScroll = -720.0f;  // ピッタリ中央で止める
            }

        // スプライトの位置を更新（Y方向にスクロール）
        Vector2 pos = { 0.0f, totalScroll };
        BGSprite_->SetPosition(pos);
    }
}

void Stage::Draw() {
	DirectXCommon* dxCommn = DirectXCommon::GetInstance();

	Sprite::PreDraw(dxCommn->GetCommandList());

	BGSprite_->Draw();

	Sprite::PostDraw();
}
