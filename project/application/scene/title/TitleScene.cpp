#include "TitleScene.h"
#include "SceneManager.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "CameraManager.h"
#include "PostEffectManager.h"
#include "GlobalVariables.h"
#ifdef USE_IMGUI
#include <imgui.h>
#endif // USE_IMGUI
#include "LightManager.h"

#include "TitleSceneStateMain.h"

void TitleScene::ChangeState(std::unique_ptr<BaseSceneState<TitleScene>> newState) {
	state_ = std::move(newState);
	state_->Initialize(this);
}

void TitleScene::Initialize(){
	BaseScene::Initialize();

	CameraManager::GetInstance()->FindCamera("default");
	CameraManager::GetInstance()->GetCamera()->SetRotate({ 0.0f,0.0f,0.0f });
	CameraManager::GetInstance()->GetCamera()->SetTranslate({ 8.0f,4.0f,-8.0f });
	camera_ = CameraManager::GetInstance()->GetCamera();

	ModelManager::GetInstance()->LoadModel("skydome/skydome.obj");
	ModelManager::GetInstance()->LoadModel("ground/ground.obj");
	TextureManager::GetInstance()->LoadTexture("AStart.png");

	//天球
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize();
	//地面
	ground_ = std::make_unique<Ground>();
	ground_->Initialize();

	ChangeState(std::make_unique<TitleSceneStateMain>());
}



void TitleScene::Update(){
#ifdef _DEBUG
	//// ウインドウフラグに NoResize を指定
	//ImGui::Begin("Settings", NULL, ImGuiWindowFlags_NoResize);
	//ImGui::ShowDemoWindow();
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	globalVariables->Update();
	std::string groupName = "";
	if (ImGui::Begin("Global Variables", nullptr, ImGuiWindowFlags_MenuBar)) {
		if (ImGui::BeginMenuBar()) {
			input_->ImGuiUpdate();
			CameraManager::GetInstance()->ImGuiUpdate();

			LightManager::GetInstance()->ImGuiUpdate();

			PostEffectManager::GetInstance()->ImGuiUpdate();
			ImGui::EndMenuBar();
		}
		ImGui::End();
	}
#endif //_DEBUG
	BaseScene::Update();

	if (state_) {
		state_->Update();
	}
}

void TitleScene::Draw(){
	if (state_) {
		state_->Draw();
	}
}
