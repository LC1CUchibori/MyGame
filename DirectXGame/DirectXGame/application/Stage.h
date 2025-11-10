#pragma once
#include <KamataEngine.h>

using namespace KamataEngine;

class Stage {
public:
    Stage();
    ~Stage();

    void Initialize();
    void Update();
    void Draw();

private:
    Sprite* BGSprite_ = nullptr;
    Sprite* LoopBGSprite1_ = nullptr;
    Sprite* LoopBGSprite2_ = nullptr;

    uint32_t BGTextureHandle_ = 0;
    uint32_t LoopBGTextureHandle_ = 0;

    float scrollSpeed = 4.0f;

    float BGHeight = 1440.0f;       // 固定背景高さ
    float LoopBGHeight = 720.0f;    // ループ背景高さ
};
