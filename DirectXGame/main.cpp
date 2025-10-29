#include "KamataEngine.h"
#include "DirectXGame/engine/secne/GameScene.h"
#include "DirectXGame/engine/secne/TitleScene.h"
#include "DirectXGame/engine/2d/Fade.h"     // ← フェード用クラス追加
#include <Windows.h>

using namespace KamataEngine;

GameScene* gameScene = nullptr;
TitleScene* titleScene = nullptr;
Fade fade;  // フェード用インスタンス

enum class Scene {
    kUnknown = 0,
    kTitleScene,
    kGame,
};

// 現在シーン
Scene scene = Scene::kUnknown;

// シーン切り替え処理
void ChangeScene();
// シーンの更新
void UpdateScene();
// シーンの描画
void DrawScene();


// Windowsアプリでのエントリーポイント
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

    // エンジン初期化
    KamataEngine::Initialize(L"LE3C_02_ウチボリ_ユウタ");
    DirectXCommon* dxCommon = DirectXCommon::GetInstance();

    // フェード初期化
    fade.Initialize();

    // 最初のシーン
    scene = Scene::kTitleScene;
    titleScene = new TitleScene;
    titleScene->Initialize();

    // メインループ
    while (true) {
        if (KamataEngine::Update()) {
            break;
        }

        // フェード更新
        fade.Update();

        // 🔸 フェード中は全シーンの更新を止める
        if (fade.IsFading()) {
            // ただし ChangeScene は動かす（フェード完了を検知するため）
            ChangeScene();

            // --- 描画だけは行う ---
            dxCommon->PreDraw();
            DrawScene();  // 現在のシーンを描画
            fade.Draw();  // その上にフェードを描画
            dxCommon->PostDraw();
            continue;  // ← 更新スキップ
        }

        // シーン切り替え処理
        ChangeScene();

        // シーン更新処理
        UpdateScene();

        // 描画開始
        dxCommon->PreDraw();

        DrawScene();

        // 🔸 フェード描画（最後に上に重ねる）
        fade.Draw();

        // 描画終了
        dxCommon->PostDraw();
    }

    // 終了処理
    KamataEngine::Finalize();

    delete gameScene;
    gameScene = nullptr;

    delete titleScene;
    titleScene = nullptr;

    return 0;
}


void ChangeScene() {
    static bool isTransitioning = false;

    switch (scene)
    {
    case Scene::kTitleScene:
        if (!isTransitioning && Input::GetInstance()->TriggerKey(DIK_RETURN)) {
            fade.StartFadeOut();
            isTransitioning = true;
        }
        if (isTransitioning && fade.IsFadeOutEnd()) {
            delete titleScene;
            titleScene = nullptr;

            gameScene = new GameScene();
            gameScene->Initialize();

            scene = Scene::kGame;

            fade.StartFadeIn();
            isTransitioning = false;
        }
        break;

    case Scene::kGame:
        if (!isTransitioning && gameScene->IsGameOver() && Input::GetInstance()->TriggerKey(DIK_RETURN)) {
            fade.StartFadeOut();
            isTransitioning = true;
        }

        if (isTransitioning && fade.IsFadeOutEnd()) {
            delete gameScene;
            gameScene = nullptr;

            titleScene = new TitleScene();
            titleScene->Initialize();

            scene = Scene::kTitleScene;

            fade.StartFadeIn();
            isTransitioning = false;
        }
        break;
    }
}

void UpdateScene()
{
    switch (scene)
    {
    case Scene::kTitleScene:
        titleScene->Update();
        break;
    case Scene::kGame:
        gameScene->Update();
        break;
    }
}

void DrawScene()
{
    switch (scene)
    {
    case Scene::kTitleScene:
        titleScene->Draw();
        break;
    case Scene::kGame:
        gameScene->Draw();
        break;
    }
}
