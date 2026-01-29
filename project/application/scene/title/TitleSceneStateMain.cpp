#include "TitleSceneStateMain.h"
#include "TitleScene.h"
#include "Input.h"
#include "SceneManager.h"
#include "CameraManager.h"
#include "PostEffectManager.h"

#include "TitleUI.h"

void TitleSceneStateMain::Initialize(TitleScene* scene) {
	BaseSceneState<TitleScene>::Initialize(scene);
	
	uiManager_ = std::make_unique<UIManager>();

	// Title UI
	std::unique_ptr<TitleUI> titleUI = std::make_unique<TitleUI>();
	titleUI->Initialize();
	uiManager_->AddUI(std::move(titleUI));
}

void TitleSceneStateMain::Update() {
	// 各メンバの更新
	scene_->GetSkydome()->Update();
	scene_->GetGround()->Update();
	uiManager_->Update();

	// カメラの回転
	auto camera = CameraManager::GetInstance()->GetCamera();
	camera->SetRotate(Vector3::Add(camera->GetRotate(), { 0.0f, 0.001f, 0.0f }));
	camera->Update(); // TitleScene.cppではここで呼んでいたが、Scene側でも呼んでいるかもしれないので確認。
	// TitleScene.cpp 81行目と94行目で呼んでいる。81はImGui前のようだが。
	// Scene::Updateでは BaseScene::Update() (Input更新) のあと、遷移チェック、UI更新、カメラ回転、カメラ更新、天球更新、地面更新、PostEffect更新。

	// シーン遷移
	if (Input::GetInstance()->TriggerKey(DIK_SPACE) || Input::GetInstance()->TriggerControllerButton(XINPUT_GAMEPAD_A)) {
		SceneManager::GetInstance()->ChangeScene("GAME");
		SceneManager::GetInstance()->ChangeTransition("FADE");
	}
}

void TitleSceneStateMain::Draw() {
	scene_->GetSkydome()->Draw();
	scene_->GetGround()->Draw();
	uiManager_->Draw();
}
