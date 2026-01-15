#include "GameOverScene.h"
#include "SceneManager.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "CameraManager.h"

void GameOverScene::Initialize(){
	BaseScene::Initialize();

	CameraManager::GetInstance()->FindCamera("default");
	CameraManager::GetInstance()->GetCamera()->SetRotate({ 0.0f,0.0f,0.0f });
	CameraManager::GetInstance()->GetCamera()->SetTranslate({ 8.0f,4.0f,-8.0f });

	ModelManager::GetInstance()->LoadModel("skydome/skydome.obj");
	ModelManager::GetInstance()->LoadModel("ground/ground.obj");
	TextureManager::GetInstance()->LoadTexture("gameOver.png");

	//天球
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize();
	//地面
	ground_ = std::make_unique<Ground>();
	ground_->Initialize();

	// GameOver UI
	gameOverUI_ = std::make_unique<GameOverUI>();
	gameOverUI_->Initialize();
}

void GameOverScene::Finalize(){
	gameOverUI_->Finalize();
	ground_.reset();
	skydome_.reset();
	BaseScene::Finalize();
}

void GameOverScene::Update(){
	BaseScene::Update();


	if ((input_->TriggerKey(DIK_SPACE) || input_->TriggerControllerButton(XINPUT_GAMEPAD_A)) &&
		sceneManager_->IsSceneAlive("FADE_OUT") == false && sceneManager_->IsSceneAlive("FADE_IN") == false) {
		sceneManager_->AddScene("FADE_OUT");
	}
	if (sceneManager_->IsSceneFinished("FADE_OUT") && sceneManager_->IsSceneAlive("FADE_IN") == false) {
		sceneManager_->RemoveScene("GAMEOVER");
		sceneManager_->RemoveScene("FADE_OUT");
		sceneManager_->AddScene("TITLE");
		sceneManager_->AddScene("FADE_IN");
	}

	//カメラの更新
	CameraManager::GetInstance()->GetCamera()->Update();

	//天球の更新
	skydome_->Update();
	//地面の更新
	//地面の更新
	ground_->Update();

	gameOverUI_->Update();
}

void GameOverScene::Draw(){
	skydome_->Draw();
	ground_->Draw();

	gameOverUI_->Draw();
}
