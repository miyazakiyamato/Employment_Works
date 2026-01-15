#include "PlayerWinScene.h"
#include "SceneManager.h"
#include "TimeManager.h"
#include "CameraManager.h"

void PlayerWinScene::Initialize(){
	BaseScene::Initialize();

	// Player Win UI
	playerWinUI_ = std::make_unique<PlayerWinUI>();
	playerWinUI_->Initialize();
}

void PlayerWinScene::Finalize(){
	//解放
	playerWinUI_->Finalize();

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

   playerWinUI_->Update();
}

void PlayerWinScene::Draw(){
	playerWinUI_->Draw();
}
