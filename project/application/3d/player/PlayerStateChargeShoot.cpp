#include "PlayerStateChargeShoot.h"
#include "Player.h"
#include "Input.h"
#include "TimeManager.h"
#include "PlayerStateRoot.h"
#include "PlayerBullet.h"
#include "EmitterSphere.h"
#include "ParticleSystem.h"

// 初期化
void PlayerStateChargeShoot::Initialize(){
	input_ = Input::GetInstance();
	timeManager_ = TimeManager::GetInstance();
	player_ = static_cast<Player*>(character_);
}

// 更新
void PlayerStateChargeShoot::Update(){
	player_->Move();

	// チャージエフェクト更新
	EmitterSphere* chargeEffect = static_cast<EmitterSphere*>(player_->GetParticleSystem()->FindEmitter("chargeEffect"));
	chargeEffect->SetTranslate(player_->GetObject3d()->GetCenterPosition());
	chargeEffect->SetIsEmitUpdate(true);
	
	// チャージ継続
	Player::AttackData attackData = player_->GetAttackData();
	attackData.chargeCount += TimeManager::GetInstance()->deltaTime_;
	player_->SetAttackData(attackData);

	// 発射
	if (!input_->PushKey(DIK_SPACE) && !input_->PushControllerButton(XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
		// 弾の速度
		Vector3 velocity(0, 0, attackData.kBulletSpeed);
		velocity = Vector3::Subtract(player_->GetReticle3d()->GetCenterPosition(), player_->GetObject3d()->GetCenterPosition());
		velocity = Vector3::Multiply(attackData.kBulletSpeed, Vector3::Normalize(velocity));
		// 弾の生成
		std::unique_ptr<BaseBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(player_->GetWorldPosition(), velocity);
		newBullet->GetObject3d()->SetScale({ attackData.chargeCount,attackData.chargeCount,attackData.chargeCount });
		player_->AddBullet(std::move(newBullet));

		// 次の弾への初期化
		attackData.chargeCount = 0.0f;
		player_->SetAttackData(attackData);
		chargeEffect->SetIsEmitUpdate(false);
		player_->ChangeState(std::make_unique<PlayerStateRoot>(character_));
	}
}
