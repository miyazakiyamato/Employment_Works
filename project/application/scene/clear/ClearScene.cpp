#include "ClearScene.h"
#include "SceneManager.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "CameraManager.h"
#include "Easing.h"
#include "TimeManager.h"
#include "PlayerStateClear.h"

void ClearScene::Initialize(){
	BaseScene::Initialize();

	CameraManager::GetInstance()->FindCamera("default");
	CameraManager::GetInstance()->GetCamera()->SetRotate({ 0.0f,0.0f,0.0f });
	CameraManager::GetInstance()->GetCamera()->SetTranslate({ 8.0f,4.0f,-8.0f });

	ModelManager::GetInstance()->LoadModel("skydome/skydome.obj");
	ModelManager::GetInstance()->LoadModel("ground/ground.obj");
	ModelManager::GetInstance()->LoadModel("airship/airship.obj");
	TextureManager::GetInstance()->LoadTexture("clear.png");

	//天球
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize();
	//地面
	ground_ = std::make_unique<Ground>();
	ground_->Initialize();
	//プレイヤー
	player_ = std::make_unique<Player>();
	player_->Initialize();
	player_->ChangeState(std::make_unique<PlayerStateClear>(player_.get()));
	// Clear UI
	clearUI_ = std::make_unique<ClearUI>();
	clearUI_->Initialize();
}

void ClearScene::Finalize(){
	clearUI_->Finalize();
	player_.reset();
	ground_.reset();
	skydome_.reset();
	BaseScene::Finalize();
}

void ClearScene::Update() {
	BaseScene::Update();


	if (input_->TriggerKey(DIK_SPACE) || input_->TriggerControllerButton(XINPUT_GAMEPAD_A)){
		sceneManager_->ChangeScene("TITLE");
		sceneManager_->ChangeTransition("FADE");
	}

	//カメラの更新
	CameraManager::GetInstance()->GetCamera()->Update();

	//天球の更新
	skydome_->Update();
	//地面の更新
	ground_->Update();
	//プレイヤーの更新
	player_->Update();

	clearUI_->Update();
}

void ClearScene::Draw(){
	skydome_->Draw();
	ground_->Draw();
	player_->Draw();
	
	clearUI_->Draw();
}
