#include "GameOverScene.h"
#include "TitleScene.h"
using namespace KamataEngine;

GameOverScene::GameOverScene() {}
GameOverScene::~GameOverScene() {
    delete gameOverSprite_;
}

void GameOverScene::Initialize() {
    gameOverTextureHandle_ = TextureManager::Load("GameOver.png");
    gameOverSprite_ = Sprite::Create(gameOverTextureHandle_, { 400.0f, 250.0f });
    timer_ = 0.0f;
    isFinished_ = false;
}

void GameOverScene::Update() {
    timer_ += 1.0f / 60.0f;

    // 少し待ってからキー入力を受け付ける
    if (timer_ > 1.0f && Input::GetInstance()->TriggerKey(DIK_SPACE)) {
        isFinished_ = true;
    }
}

void GameOverScene::Draw() {
    DirectXCommon* dxCommon = DirectXCommon::GetInstance();

    Sprite::PreDraw(dxCommon->GetCommandList());
    gameOverSprite_->Draw();
    Sprite::PostDraw();
}
