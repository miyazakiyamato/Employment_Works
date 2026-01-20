
#include "PlayerDeathScene.h"
#include "SceneManager.h"
#include "TimeManager.h"
#include "CameraManager.h"

void PlayerDeathScene::Initialize(){
	BaseScene::Initialize();

	// Player Death UI
	playerDeathUI_ = std::make_unique<PlayerDeathUI>();
	playerDeathUI_->Initialize();
}

void PlayerDeathScene::Finalize(){
	//解放
	playerDeathUI_->Finalize();

	BaseScene::Finalize();
}


void PlayerDeathScene::Update() {
   BaseScene::Update();
   counter_ += TimeManager::GetInstance()->kFlamTime_;

   if (counter_ < duration_) {
      
   }
   else if (isFinished_ == false){
	   isFinished_ = true;
       //sceneManager_->AddScene("FADE_OUT");
   }

   playerDeathUI_->Update();
}

void PlayerDeathScene::Draw(){
	playerDeathUI_->Draw();
}
