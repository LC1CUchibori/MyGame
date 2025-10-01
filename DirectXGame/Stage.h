#pragma once
#include <KamataEngine.h>

class Stage
{
public:
    Stage();
    ~Stage();

    void Initialize();

    void Update();

    void Draw();

private:
    // 元背景
    uint32_t BGTextureHandle_ = 0;
    KamataEngine::Sprite* BGSprite_ = nullptr;
    float totalScroll = 0.0f;
    float scrollSpeed = 2.0f;

    // 無限スクロール背景用
    uint32_t LoopBGTextureHandle_ = 0;
    KamataEngine::Sprite* LoopBGSprite1_ = nullptr;
    KamataEngine::Sprite* LoopBGSprite2_ = nullptr;

};
