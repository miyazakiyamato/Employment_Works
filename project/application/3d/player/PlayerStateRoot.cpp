#include "PlayerStateRoot.h"
#include "Player.h"
#include "Input.h"
#include "TimeManager.h"
#include "PlayerStateShoot.h"
#include "PlayerStateChargeShoot.h"

// 初期化
void PlayerStateRoot::Initialize(){
	input_ = Input::GetInstance();
	timeManager_ = TimeManager::GetInstance();
	player_ = static_cast<Player*>(character_);
}

// 更新
void PlayerStateRoot::Update(){
	player_->Move();
	player_->ReticleUpdate();

	Player::AttackData attackData = player_->GetAttackData();
	if (input_->PushKey(DIK_SPACE) || input_->PushControllerButton(XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
		attackData.chargeCount += TimeManager::GetInstance()->deltaTime_;
		if (attackData.chargeCount >= attackData.kChargeTime) {
			player_->SetAttackData(attackData);
			player_->ChangeState(std::make_unique<PlayerStateChargeShoot>(character_));
			return;
		}
	} else {
		if (attackData.chargeCount != 0.0f) {
			attackData.chargeCount = 0.0f;
			player_->SetAttackData(attackData);
			player_->ChangeState(std::make_unique<PlayerStateShoot>(character_));
			return;
		}
		attackData.chargeCount = 0.0f;
	}
	player_->SetAttackData(attackData);
}
