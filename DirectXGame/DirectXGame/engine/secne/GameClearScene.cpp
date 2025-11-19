#include "GameClearScene.h"
#include "TitleScene.h"
using namespace KamataEngine;

GameClearScene::GameClearScene() {}
GameClearScene::~GameClearScene() {
    delete gameClearSprite_;
    delete gameClearBGSprite_;
}

void GameClearScene::Initialize() {
    // TODO: GameClear専用の画像を作成して使用する
    gameClearTextureHandle_ = TextureManager::Load("GameOver.png");
    gameClearSprite_ = Sprite::Create(gameClearTextureHandle_, { 400.0f, 250.0f });

    gameClearBGTextureHandle_ = TextureManager::Load("ClearBG.png");
    gameClearBGSprite_ = Sprite::Create(gameClearBGTextureHandle_, { 0.0f,0.0f });

    timer_ = 0.0f;
    isFinished_ = false;
}

void GameClearScene::Update() {
    timer_ += 1.0f / 60.0f;

    // 少し待ってからキー入力を受け付ける
    if (timer_ > 1.0f && Input::GetInstance()->TriggerKey(DIK_SPACE)) {
        isFinished_ = true;
    }
}

void GameClearScene::Draw() {
    DirectXCommon* dxCommon = DirectXCommon::GetInstance();

    Sprite::PreDraw(dxCommon->GetCommandList());
    gameClearSprite_->Draw();

    gameClearBGSprite_->Draw();
    Sprite::PostDraw();
}
