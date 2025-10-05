#include "Enemy.h"
#include <CollisionTypeIdDef.h>
#include "ParticleSystem.h"
#include "PlayerBullet.h"
#include "BulletManager.h"
#include "EnemyBullet.h"
#include "Player.h"
#include <cassert>

void Enemy::Initialize(){
	BaseCharacter::Initialize();
	Collider::Initialize();
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kEnemy));
	Collider::SetRadius(1.0f);
	object3d_->SetTranslate({ 0.0f, 1.0f, 30.0f });
	object3d_->SetModel("sphere/sphere.obj");
	object3d_->SetRotate({ 0,3.14f,0 });
	object3d_->SetColor({ 0.0f, 0.0f, 0.0f, 1.0f },0);

	FireTimed();
}

void Enemy::Update(){
	//
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
	BaseCharacter::Update();
}

void Enemy::Draw(){
	object3d_->Draw();
}

void Enemy::OnCollision(Collider* other){
	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeID();
	//衝突相手が敵なら
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kPlayerBullet)) {
		PlayerBullet* playerBullet = static_cast<PlayerBullet*>(other);
		Vector3 distance = playerBullet->GetCenterPosition() - object3d_->GetCenterPosition();
		distance = distance.Normalize() * GetRadius();
		particleSystem_->FindEmitter("hitEffect")->SetPosition(object3d_->GetCenterPosition() + distance);
		particleSystem_->Emit("hitEffect");
		hp_ -= 1;
		if (hp_ <= 0) {
			isAlive_ = false;
		}
	}
}
void Enemy::FireTimed() {
	Fire();
	//
	timedCalls_.push_back(new TimedCall(std::bind_front(&Enemy::FireTimed, this), kFireInterval));
}

void Enemy::FireCancel() {
	timedCalls_.remove_if([](TimedCall* timedCalls) {
		delete timedCalls;
		return true;
		});
}

void Enemy::Fire() {
	assert(player_);
	//
	const float kBulletSpeed = 100.0f;

	Vector3 velocity{ player_->GetWorldPosition() - object3d_->GetCenterPosition() };
	velocity = Vector3::Multiply(kBulletSpeed, velocity.Normalize());

	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(object3d_->GetCenterPosition(), velocity);
	bulletManager_->AddBullet(std::move(newBullet));
}
