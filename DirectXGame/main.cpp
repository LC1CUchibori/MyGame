#include "KamataEngine.h"
#include "DirectXGame/engine/secne/GameScene.h"
#include "DirectXGame/engine/secne/TitleScene.h"
#include "DirectXGame/engine/2d/Fade.h"
#include "DirectXGame/engine/secne/GameClearScene.h"
#include <Windows.h>

using namespace KamataEngine;

GameScene* gameScene = nullptr;
TitleScene* titleScene = nullptr;
GameClearScene* gameClearScene = nullptr;
Fade fade;  // フェード用インスタンス

enum class Scene {
    kUnknown = 0,
    kTitleScene,
    kGame,
    kGameClear,
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

    //WinApp::GetInstance()->SetFullscreen(true);

   
    ImGuiManager* imGuiManger_ = ImGuiManager::GetInstance();

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


        imGuiManger_->Begin();
        if (scene==Scene::kGame)
        {

        gameScene->DrawImGui();
        }

        imGuiManger_->End();

        // フェード中は全シーンの更新を止める
        if (fade.IsFading()) {
            // ChangeScene は動かす
            ChangeScene();

            // --- 描画だけは行う ---
            dxCommon->PreDraw();
            DrawScene();  // 現在のシーンを描画
            fade.Draw();  // その上にフェードを描画
            dxCommon->PostDraw();
            continue;  // 更新スキップ
        }

        // シーン切り替え処理
        ChangeScene();

        // シーン更新処理
        UpdateScene();

        // 描画開始
        dxCommon->PreDraw();

        DrawScene();

        // フェード描画
        fade.Draw();

        imGuiManger_->Draw();

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
        if (!isTransitioning && Input::GetInstance()->TriggerKey(DIK_SPACE)) {
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
        if (!gameScene) break; // nullptrチェック

        // --------------------- クリアしたらクリア画面へ ---------------------
        if (!isTransitioning && gameScene->IsGameClear()) {
            fade.StartFadeOut();
            isTransitioning = true;
        }
        if (isTransitioning && fade.IsFadeOutEnd() && gameScene->IsGameClear()) {

            delete gameScene;
            gameScene = nullptr;

            gameClearScene = new GameClearScene();
            gameClearScene->Initialize();

            scene = Scene::kGameClear;

            fade.StartFadeIn();
            isTransitioning = false;
        }

        // --------------------- ゲームオーバーへ---------------------
        if (!isTransitioning && gameScene && gameScene->IsGameOver()) {
            // まずゲームオーバー画像表示だけ
            // ENTER を待つ
            if (Input::GetInstance()->TriggerKey(DIK_RETURN)) {
                fade.StartFadeOut();
                isTransitioning = true;
            }
        }

        if (isTransitioning && fade.IsFadeOutEnd() && gameScene && gameScene->IsGameOver()) {

            delete gameScene;
            gameScene = nullptr;

            titleScene = new TitleScene();
            titleScene->Initialize();

            scene = Scene::kTitleScene;

            fade.StartFadeIn();
            isTransitioning = false;
        }

        // --------------------- タイトルに戻る ---------------------
        if (!isTransitioning && gameScene && gameScene->IsReturnToTitle()) {

            fade.StartFadeOut();
            isTransitioning = true;
        }

        if (isTransitioning && fade.IsFadeOutEnd()
            && gameScene && gameScene->IsReturnToTitle()) {

            delete gameScene;
            gameScene = nullptr;

            titleScene = new TitleScene();
            titleScene->Initialize();

            scene = Scene::kTitleScene;

            fade.StartFadeIn();
            isTransitioning = false;
        }
        break;

        // --------------------- クリア画面からタイトルへ ---------------------
    case Scene::kGameClear:
        if (!isTransitioning && Input::GetInstance()->TriggerKey(DIK_RETURN)) {
            fade.StartFadeOut();
            isTransitioning = true;
        }
        if (isTransitioning && fade.IsFadeOutEnd()) {

            delete gameClearScene;
            gameClearScene = nullptr;

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
        if (titleScene) titleScene->Update();
        break;
    case Scene::kGame:
        if (gameScene) gameScene->Update();
        break;
    case Scene::kGameClear:
        if (gameClearScene) gameClearScene->Update();
        break;

    }
}

void DrawScene()
{
    switch (scene)
    {
    case Scene::kTitleScene:
        if (titleScene) titleScene->Draw();
        break;
    case Scene::kGame:
        if (gameScene) gameScene->Draw();
        break;
    case Scene::kGameClear:
        if (gameClearScene) gameClearScene->Draw();
        break;
    }
}
