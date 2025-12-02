#include "PlayerStateShoot.h"
#include "Player.h"
#include "Input.h"
#include "TimeManager.h"
#include "PlayerStateRoot.h"
#include "PlayerBullet.h"

// 初期化
void PlayerStateShoot::Initialize(){
	input_ = Input::GetInstance();
	timeManager_ = TimeManager::GetInstance();
	player_ = static_cast<Player*>(character_);
}

// 更新
void PlayerStateShoot::Update(){
	player_->Move();
	player_->ReticleUpdate();
	
	// 発射
	Player::AttackData attackData = player_->GetAttackData();
	attackData.chargeCount += TimeManager::GetInstance()->deltaTime_;
	if (attackData.bulletCount == 0 ||
		attackData.chargeCount >= attackData.kChargeTime) {
		// 弾の速度
		Vector3 velocity(0, 0, attackData.kBulletSpeed);
		velocity = Vector3::Subtract(player_->GetReticle3d()->GetCenterPosition(),player_->GetObject3d()->GetCenterPosition());
		velocity = Vector3::Multiply(attackData.kBulletSpeed, Vector3::Normalize(velocity));
		// 弾の生成
		std::unique_ptr<BaseBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(player_->GetWorldPosition(), velocity);
		newBullet->GetObject3d()->SetScale({ attackData.kChargeTime,attackData.kChargeTime,attackData.kChargeTime });
		player_->AddBullet(std::move(newBullet));

		// 次の弾への初期化
		attackData.bulletCount++;
		attackData.chargeCount = 0.0f;
	}
	player_->SetAttackData(attackData);

	// 全弾発射したら状態遷移
	if (attackData.bulletCount >= attackData.kBulletCount) {
		attackData.bulletCount = 0;
		player_->SetAttackData(attackData);
		player_->ChangeState(std::make_unique<PlayerStateRoot>(character_));
	}
}
