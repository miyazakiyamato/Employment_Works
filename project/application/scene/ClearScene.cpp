#include "ClearScene.h"
#include "SceneManager.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "CameraManager.h"
#include "Easing.h"
#include <TimeManager.h>

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
	//スプライトの初期化
	for (uint32_t i = 0; i < 1; ++i) {
		std::unique_ptr<Sprite> sprite(new Sprite);
		sprite->Initialize("clear.png");
		sprite->SetPosition({640, 260 });
		sprite->SetSize({ 360.0f,140.0f });
		sprite->SetAnchorPoint({ 0.5f, 0.5f });
		sprites_.push_back(std::move(sprite));
	}
}

void ClearScene::Finalize(){
	for (std::unique_ptr<Sprite>& sprite : sprites_) {
		sprite.reset();  // メモリを解放する
	}
	player_.reset();
	ground_.reset();
	skydome_.reset();
	BaseScene::Finalize();
}

void ClearScene::Update() {
	BaseScene::Update();


	if ((input_->TriggerKey(DIK_SPACE) || input_->TriggerControllerButton(XINPUT_GAMEPAD_A)) &&
		sceneManager_->IsSceneAlive("FADE_OUT") == false && sceneManager_->IsSceneAlive("FADE_IN") == false) {
		sceneManager_->AddScene("FADE_OUT");
	}
	if (sceneManager_->IsSceneFinished("FADE_OUT") && sceneManager_->IsSceneAlive("FADE_IN") == false) {
		sceneManager_->RemoveScene("CLEAR");
		sceneManager_->RemoveScene("FADE_OUT");
		sceneManager_->AddScene("TITLE");
		sceneManager_->AddScene("FADE_IN");
	}

	//カメラの更新
	CameraManager::GetInstance()->GetCamera()->Update();

	//天球の更新
	skydome_->Update();
	//地面の更新
	ground_->Update();
	//プレイヤーの更新
	player_->ClearUpdate();

	scaleCount_ += TimeManager::GetInstance()->deltaTime_;
	if (scaleCount_ > 1.0f) {
		scaleCount_ = 0.0f;
	}
	float scale{};
	if (scaleCount_ <= 0.5f) {
		scale = Easing::EaseInOutSine(scaleCount_ * 2.0f, 0.5f, 1.0f);
	} else {
		scale = Easing::EaseInOutSine(scaleCount_ * 2.0f - 1.0f, 1.0f, 0.5f);
	}
	sprites_[0]->SetSize(Vector2(360.0f, 140.0f) *scale);
	for (std::unique_ptr<Sprite>& sprite : sprites_) {
		sprite->Update();
	}
}

void ClearScene::Draw(){
	skydome_->Draw();
	ground_->Draw();
	player_->Draw();
	for (std::unique_ptr<Sprite>& sprite : sprites_) {
		sprite->Draw();
	}
}
