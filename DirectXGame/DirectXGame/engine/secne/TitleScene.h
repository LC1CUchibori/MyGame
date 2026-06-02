#pragma once

#include <cstdint>
#include <memory>
#include <KamataEngine.h>
#include "../../application/Stage.h"
#include "../3d/TitleModel.h"
#include "BaseScene.h"

class TitleScene: public BaseScene {
public:
    // シングルトン取得
    static TitleScene* GetInstance();

    // コピー禁止
    TitleScene(const TitleScene&) = delete;
    TitleScene& operator=(const TitleScene&) = delete;

    void Initialize()override;
    void Update()override;
    void Draw()override;

private:
    // コンストラクタを隠す
    TitleScene();
    ~TitleScene();

private:
    // モデル
    KamataEngine::Model* model_ = nullptr;

    // ワールドトランスフォーム
    KamataEngine::WorldTransform worldTransform_;

    // カメラ
    KamataEngine::Camera camera_;

    // インプット
    KamataEngine::Input* input = KamataEngine::Input::GetInstance();

    // オーディオ
    KamataEngine::Audio* audio = KamataEngine::Audio::GetInstance();

    // タイトル操作テクスチャハンドル
    uint32_t HitTextureHandle_ = 0;

    // タイトル操作スプライト
    KamataEngine::Sprite* HitSprite_ = nullptr;

    // タイトルスプライト
    uint32_t TitleTextureHandle_ = 0;
    KamataEngine::Sprite* TitleSprite_ = nullptr;

    // STARTランプ
    KamataEngine::Sprite* gogoOFFSprite_ = nullptr;
    KamataEngine::Sprite* gogoONSprite_ = nullptr;
    uint32_t gogoOFFTextureHandle_ = 0;
    uint32_t gogoONTextureHandle_ = 0;
    bool isGogoOn_ = false;

    // ステージ
    std::unique_ptr<Stage> stage1 = nullptr;

    // タイトルモデル
    std::unique_ptr<TitleModel> title_;
    std::unique_ptr<KamataEngine::Model> titleModel_;

    float hitAlphaTime_ = 0.0f;

    // ペカリ音
    uint32_t startSoundHandle_ = 0;
};