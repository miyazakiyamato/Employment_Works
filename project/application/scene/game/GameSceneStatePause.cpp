#include "GameSceneStatePause.h"
#include "GameScene.h"
#include "Input.h"
#ifdef USE_IMGUI
#include <imgui.h>
#endif

void GameSceneStatePause::Initialize(GameScene* gameScene) {
	BaseSceneState<GameScene>::Initialize(gameScene);
}

void GameSceneStatePause::Initialize(GameScene* gameScene, std::unique_ptr<BaseSceneState<GameScene>> previousState) {
	Initialize(gameScene);
	previousState_ = std::move(previousState);
}

void GameSceneStatePause::Update() {
	// ポーズ解除
	if (Input::GetInstance()->TriggerKey(DIK_P) || Input::GetInstance()->TriggerControllerButton(XINPUT_GAMEPAD_START)) {
		// ステートを元に戻す
		scene_->ChangeState(std::move(previousState_));
		return;
	}
}

void GameSceneStatePause::Draw() {
	// ゲーム画面を背景として描画
	scene_->DrawGame3D();
}
