#include "PlayerStateLeave.h"
#include "Player.h"
#include "Input.h"
#include "TimeManager.h"
#include "PlayerStateRoot.h"
#include "PlayerBullet.h"

// 初期化
void PlayerStateLeave::Initialize(){
	input_ = Input::GetInstance();
	timeManager_ = TimeManager::GetInstance();
	player_ = static_cast<Player*>(character_);
}

// 更新
void PlayerStateLeave::Update(){
	Object3d* playerObject = player_->GetObject3d();
	playerObject->SetTranslate(playerObject->GetTranslate() + leaveVelocity_ * timeManager_->deltaTime_);
}
