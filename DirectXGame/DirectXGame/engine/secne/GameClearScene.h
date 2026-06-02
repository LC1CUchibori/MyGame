#pragma once
#include <KamataEngine.h>
#include "../../application/Stage.h"
#include "BaseScene.h"

class GameClearScene : public BaseScene {
public:
    GameClearScene();
    ~GameClearScene();

    void Initialize() override;
    void Update() override;
    void Draw()override;

    bool IsFinished() const { return isFinished_; }

private:
    uint32_t gameClearTextureHandle_ = 0;
    KamataEngine::Sprite* gameClearSprite_ = nullptr;

    uint32_t warningTextureHandle_ = 0;
    KamataEngine::Sprite* warningSprite_ = nullptr;

    Stage* stage_ = nullptr;

    bool isFinished_ = false;
    float timer_ = 0.0f;

    float warningY_ = -250.0f;     // 最初画面外

    float dropSpeed_ = 5.0f; 
};

