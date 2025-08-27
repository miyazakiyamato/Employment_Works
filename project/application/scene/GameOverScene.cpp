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

	//スプライトの初期化
	for (uint32_t i = 0; i < 1; ++i) {
		std::unique_ptr<Sprite> sprite(new Sprite);
		sprite->Initialize("gameOver.png");
		sprite->SetPosition({640, 260 });
		sprite->SetSize({ 720.0f,140.0f });
		sprite->SetAnchorPoint({ 0.5f, 0.5f });
		sprites_.push_back(std::move(sprite));
	}
}

void GameOverScene::Finalize(){
	for (std::unique_ptr<Sprite>& sprite : sprites_) {
		sprite.reset();  // メモリを解放する
	}
	ground_.reset();
	skydome_.reset();
	BaseScene::Finalize();
}

void GameOverScene::Update(){
	BaseScene::Update();


	if (input_->TriggerKey(DIK_SPACE) || input_->TriggerControllerButton(XINPUT_GAMEPAD_A)) {
		sceneManager_->ChangeScene("TITLE");
	}

	//カメラの更新
	CameraManager::GetInstance()->GetCamera()->Update();

	//天球の更新
	skydome_->Update();
	//地面の更新
	ground_->Update();

	for (std::unique_ptr<Sprite>& sprite : sprites_) {
		sprite->Update();
	}
}

void GameOverScene::Draw(){
	skydome_->Draw();
	ground_->Draw();

	for (std::unique_ptr<Sprite>& sprite : sprites_) {
		sprite->Draw();
	}
}
