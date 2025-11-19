#pragma once
#include <KamataEngine.h>

class GameClearScene {
public:
    GameClearScene();
    ~GameClearScene();

    void Initialize();
    void Update();
    void Draw();

    bool IsFinished() const { return isFinished_; }

private:
    uint32_t gameClearTextureHandle_ = 0;
    KamataEngine::Sprite* gameClearSprite_ = nullptr;

    uint32_t gameClearBGTextureHandle_ = 0;
    KamataEngine::Sprite* gameClearBGSprite_ = nullptr;

    bool isFinished_ = false;
    float timer_ = 0.0f;
};

