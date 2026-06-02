#pragma once
#include <KamataEngine.h>

class Fade {
public:
    void Initialize();

    void Update();

    void Draw();

    void StartFadeOut();
    void StartFadeIn();

    bool IsFadeOutEnd() const { return fadeOutEnd_; }
    bool IsFadeInEnd() const { return fadeInEnd_; }
    bool IsFading() const { return isFading_; }

private:
    KamataEngine::Sprite* fadeSprite_ = nullptr;
    uint32_t fadeTextureHandle_ = 0;

    float alpha_ = 0.0f;
    bool isFading_ = false;
    bool isFadeOut_ = false;
    bool fadeOutEnd_ = false;
    bool fadeInEnd_ = false;
};
