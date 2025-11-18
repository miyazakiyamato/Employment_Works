#include "PlayerWinScene.h"
#include "SceneManager.h"
#include "TimeManager.h"
#include "CameraManager.h"

void PlayerWinScene::Initialize(){
	BaseScene::Initialize();

	//スプライトの初期化
	for (uint32_t i = 0; i < 0; ++i) {
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
}

void PlayerWinScene::Finalize(){
	//解放
	for (std::unique_ptr<Sprite>& sprite : sprites_) {
		sprite.reset();  // メモリを解放する
	}

	BaseScene::Finalize();
}


void PlayerWinScene::Update() {
   BaseScene::Update();
   counter_ += TimeManager::GetInstance()->kFlamTime_;

   if (counter_ < duration_) {
      
   }
   else if (isFinished_ == false){
	   isFinished_ = true;
       sceneManager_->AddScene("FADE_OUT");
   }

   for (std::unique_ptr<Sprite>& sprite : sprites_) {
       sprite->Update();
   }
}

void PlayerWinScene::Draw(){
	for (std::unique_ptr<Sprite>& sprite : sprites_) {
		sprite->Draw();
	}
}
