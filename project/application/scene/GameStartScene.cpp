
#include "GameStartScene.h"
#include "SceneManager.h"
#include "TimeManager.h"
#include <CameraManager.h>

void GameStartScene::Initialize(){
	BaseScene::Initialize();

	//スプライトの初期化
	for (uint32_t i = 0; i < 1; ++i) {
		std::unique_ptr<Sprite> sprite(new Sprite);
		sprite->Initialize("num.png");
		sprite->SetPosition({ 640,360 });
		sprite->SetSize({ 90.0f,144.0f });
		sprite->SetAnchorPoint({ 0.5f, 0.5f });
		sprite->SetUVTranslate({ 0.3f,0.0f });
		sprite->SetUVScale({ 0.1f,1.0f });
		sprite->SetColor({ 1.0f,1.0f,1.0f,1.0f });
		sprites_.push_back(std::move(sprite));
	}
	TimeManager::GetInstance()->SetDeltaTimeSpeedStart(0.0f, duration_ * 3.0f);
}

void GameStartScene::Finalize(){
	//解放
	for (std::unique_ptr<Sprite>& sprite : sprites_) {
		sprite.reset();  // メモリを解放する
	}

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
   if (counter_ < duration_) {
       float t = (counter_ / duration_) * 3.14159265f; // 0～πに正規化
       float alpha = std::sinf(t);
       alpha = std::abs(alpha); // 0→1→0
       sprites_[0]->SetColor({ 1.0f,1.0f,1.0f,alpha });
   }
   else {
       counter_ = 0.0f;
       sprites_[0]->SetColor({ 1.0f,1.0f,1.0f,0.0f });
       if (sprites_[0]->GetUVTranslate().x - 0.1f >= 0.1f) {
           sprites_[0]->SetUVTranslate({ sprites_[0]->GetUVTranslate().x - 0.1f,0.0f });
       } else if (sprites_[0]->GetUVTranslate().x - 0.1f >= 0.0f) {
           sprites_[0]->SetTexture("go.png");
           sprites_[0]->SetUVTranslate({ 0.0f,0.0f });
           sprites_[0]->SetUVScale({ 1.0f,1.0f });
           sprites_[0]->SetSize({ 180.0f,144.0f });
           isFinished_ = true;
       }
       else {
           sceneManager_->RemoveScene("GAME_START");
       }
   }

   for (std::unique_ptr<Sprite>& sprite : sprites_) {
       sprite->Update();
   }
}

void GameStartScene::Draw(){
	for (std::unique_ptr<Sprite>& sprite : sprites_) {
		sprite->Draw();
	}
}
