#include "KamataEngine.h"
#include "GameScene.h"
#include "TitleScene.h"
#include <Windows.h>

using namespace KamataEngine;

GameScene* gameScene = nullptr;
TitleScene* titleScene = nullptr;

enum class Scene {
	kUnknown=0,

	kTitleScene,
	kGame,
};

// 現在シーン
Scene scene = Scene::kUnknown;

//シーン切り替え処理
void ChangeScene();

//シーンの更新
void UpdateScene();

//シーンの描画
void DrawScene();


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	
	// エンジンの初期化
	KamataEngine::Initialize(L"LE3C_02_ウチボリ_ユウタ");

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	

	// 最初のシーンの初期化
	scene = Scene::kTitleScene;
	titleScene = new TitleScene;
	titleScene->Initialize();

	// メインループ
	while (true) {
		if (KamataEngine::Update()) {
			break;
		}

		ChangeScene();

		UpdateScene();

		// 描画開始
		dxCommon->PreDraw();

		DrawScene();

		// 描画終了処理
		dxCommon->PostDraw();
	}

	// エンジンの終了処理
	KamataEngine::Finalize();

	// ゲームシーンの解放
	delete gameScene;
	// nullptrの代入
	gameScene = nullptr;

	return 0;
}

void ChangeScene()
{
	switch (scene)
	{
	case Scene::kTitleScene:
		if (Input::GetInstance()->TriggerKey(DIK_RETURN)) {
			// シーンの変更
			scene = Scene::kGame;
			// 旧シーンの解放
			delete titleScene;
			titleScene = nullptr;
			// 新シーンの生成と初期化
			gameScene = new GameScene();
			gameScene->Initialize();
		}
		break;
	case Scene::kGame:
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
