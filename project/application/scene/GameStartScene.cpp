#include "GameStartScene.h"
#include "SceneManager.h"
#include "TimeManager.h"
#include "CameraManager.h"
#include "GameStartUI.h"

void GameStartScene::Initialize(){
	BaseScene::Initialize();

	// Game Start UI
	gameStartUI_ = std::make_unique<GameStartUI>();
	gameStartUI_->Initialize(duration_);

	TimeManager::GetInstance()->SetDeltaTimeSpeedStart(0.0f, duration_ * 3.0f);
}

void GameStartScene::Finalize(){
	gameStartUI_->Finalize();
	BaseScene::Finalize();
}


void GameStartScene::Update() {
   BaseScene::Update();
   counter_ += TimeManager::GetInstance()->kFlamTime_;
   // 3回分のdurationで一周
   const float totalDuration = duration_ * 3.0f;
   float progress = TimeManager::GetInstance()->kFlamTime_ / totalDuration;
   // 角度（ラジアンで一周）
   float cameraAngle = progress * 2.0f * 3.14159265f;

   // カメラに反映（RailCameraのインスタンス名をcamera_と仮定）
   CameraManager::GetInstance()->SetCamera("RailCamera");
   Camera* camera_ = CameraManager::GetInstance()->GetCamera();
   if (camera_) {
	   Vector3 rotation = camera_->GetRotate();
	   rotation.y += cameraAngle;
       camera_->SetRotate(rotation); // SetRotationがラジアンの場合
	   camera_->Update();
   }
   gameStartUI_->Update();
   if (gameStartUI_->IsFinished()) {
       isFinished_ = true;
   }
   if (gameStartUI_->ShouldClose()) {
       //sceneManager_->RemoveScene("GAME_START");
   }
}

void GameStartScene::Draw(){
	gameStartUI_->Draw();
}
