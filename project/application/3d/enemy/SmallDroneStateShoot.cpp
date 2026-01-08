#include "SmallDroneStateShoot.h"
#include "SmallDrone.h"
#include "Player.h"
#include "BulletManager.h"
#include "TimeManager.h"
#include "Easing.h"
#include "EnemyBullet.h"
#include "SmallDroneStateDeath.h"

SmallDroneStateShoot::SmallDroneStateShoot(BaseEnemy* smallDrone)
	: BaseEnemyState("SmallDrone State Shoot", smallDrone){}

void SmallDroneStateShoot::Initialize(){
	smallDrone_ = static_cast<SmallDrone*>(enemy_);
	player_ = enemy_->GetPlayer();
	bulletManager_ = enemy_->GetBulletManager();
	timeManager_ = TimeManager::GetInstance();
	// 発射開始
	timedCalls_.push_back(new TimedCall(std::bind_front(&SmallDroneStateShoot::FireTimed, this), smallDrone_->GetFireInterval()));
}

void SmallDroneStateShoot::Update(){
	// 死亡処理Stateへ移行
	if (smallDrone_->GetHP() <= 0) {
		FireCancel();
		enemy_->ChangeState(std::make_unique<SmallDroneStateDeath>(enemy_));
		return;
	}
	// 時間経過
	timedCalls_.remove_if([](TimedCall* timedCalls) {
		if (timedCalls->IsFinished()) {
			delete timedCalls;
			return true;
		}
		return false;
		});
	for (TimedCall* timedCalls : timedCalls_) {
		timedCalls->Update();
	}
}

void SmallDroneStateShoot::Fire() {
	assert(player_);
	//
	const float kBulletSpeed = 30.0f;

	Vector3 velocity{ player_->GetWorldPosition() - smallDrone_->GetObject3d()->GetCenterPosition() };
	velocity = Vector3::Multiply(kBulletSpeed, velocity.Normalize());

	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(smallDrone_->GetObject3d()->GetCenterPosition(), velocity);
	bulletManager_->AddBullet(std::move(newBullet));
}
void SmallDroneStateShoot::FireTimed() {
	Fire();
	//
	timedCalls_.push_back(new TimedCall(std::bind_front(&SmallDroneStateShoot::FireTimed, this), smallDrone_->GetFireInterval()));
}

void SmallDroneStateShoot::FireCancel() {
	timedCalls_.remove_if([](TimedCall* timedCalls) {
		delete timedCalls;
		return true;
		});
}
