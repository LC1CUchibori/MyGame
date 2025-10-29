#pragma once
#include <KamataEngine.h>

class GameOverScene {
public:
    GameOverScene();
    ~GameOverScene();

    void Initialize();
    void Update();
    void Draw();

    bool IsFinished() const { return isFinished_; }

private:
    uint32_t gameOverTextureHandle_ = 0;
    KamataEngine::Sprite* gameOverSprite_ = nullptr;

    bool isFinished_ = false;
    float timer_ = 0.0f;
};
