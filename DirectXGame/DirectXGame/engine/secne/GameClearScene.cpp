#include "GameClearScene.h"
#include "TitleScene.h"
using namespace KamataEngine;

GameClearScene::GameClearScene() {}
GameClearScene::~GameClearScene() {
    delete gameClearSprite_;
    delete stage_;
}

void GameClearScene::Initialize() {
    // ゲームクリア
    gameClearTextureHandle_ = TextureManager::Load("GameClear.png");
    gameClearSprite_ = Sprite::Create(gameClearTextureHandle_, { 400.0f, 200.0f });

    // 注意書き
    warningTextureHandle_ = TextureManager::Load("warning.png");
    warningSprite_ = Sprite::Create(warningTextureHandle_, { 400.0f,warningY_ });

    // ステージ
    stage_ = new Stage;
    stage_->Initialize();

    timer_ = 0.0f;
    isFinished_ = false;
}

void GameClearScene::Update() {
    timer_ += 1.0f / 60.0f;

    if (warningY_ < 10.0f) {
        warningY_ += dropSpeed_;
        if (warningY_ > 10.0f) warningY_ = 10.0f;
        warningSprite_->SetPosition({ 400.0f, warningY_ });
    }

    // 少し待ってからキー入力を受け付ける
    if (timer_ > 1.0f && Input::GetInstance()->TriggerKey(DIK_SPACE)) {
        isFinished_ = true;
    }

    // 背景ステージ
    stage_->Update();
}

void GameClearScene::Draw() {
    DirectXCommon* dxCommon = DirectXCommon::GetInstance();
 

    Sprite::PreDraw(dxCommon->GetCommandList());

    stage_->Draw();

    Sprite::PostDraw();

    Sprite::PreDraw(dxCommon->GetCommandList());

    gameClearSprite_->Draw();

    warningSprite_->Draw();

    Sprite::PostDraw();
}
