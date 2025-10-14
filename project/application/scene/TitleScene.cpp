#include "TitleScene.h"
#include "SceneManager.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "CameraManager.h"

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

	//スプライトの初期化
	for (uint32_t i = 0; i < 2; ++i) {
		std::unique_ptr<Sprite> sprite(new Sprite);
		sprite->SetAnchorPoint({ 0.5f, 0.5f });
		sprites_.push_back(std::move(sprite));
	}
	sprites_[0]->Initialize("title.png");
	sprites_[0]->SetPosition({ 640, 260 });
	sprites_[0]->SetSize({ 800.0f,300.0f });
	sprites_[1]->Initialize("AStart.png");
	sprites_[1]->SetPosition({ 640, 460 });
	sprites_[1]->SetSize({ 250.0f,100.0f });
}

void TitleScene::Finalize(){
	for (std::unique_ptr<Sprite>& sprite : sprites_) {
		sprite.reset();  // メモリを解放する
	}
	ground_.reset();
	skydome_.reset();
	BaseScene::Finalize();
}

void TitleScene::Update(){
	BaseScene::Update();


	if ((input_->TriggerKey(DIK_SPACE) || input_->TriggerControllerButton(XINPUT_GAMEPAD_A)) &&
		sceneManager_->IsSceneAlive("FADE_OUT") == false) {
		sceneManager_->AddScene("FADE_OUT");
	}
	if (sceneManager_->IsSceneFinished("FADE_OUT")) {
		sceneManager_->RemoveScene("TITLE");
		sceneManager_->RemoveScene("FADE_OUT");
		sceneManager_->AddScene("GAME");
		sceneManager_->AddScene("FADE_IN");
	}
	for (std::unique_ptr<Sprite>& sprite : sprites_) {
		sprite->Update();
	}
	camera_->SetRotate(Vector3::Add(camera_->GetRotate() , { 0.0f,0.001f,0.0f }));

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

void TitleScene::Draw(){
	skydome_->Draw();
	ground_->Draw();

	for (std::unique_ptr<Sprite>& sprite : sprites_) {
		sprite->Draw();
	}
}
