#include "Enemy.h"
#include <CollisionTypeIdDef.h>
#include "ParticleSystem.h"
#include "PlayerBullet.h"

void Enemy::Initialize(){
	BaseCharacter::Initialize();
	Collider::Initialize();
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kEnemy));
	Collider::SetRadius(1.0f);
	object3D_->SetTranslate({ 0.0f, 1.0f, 30.0f });
	object3D_->SetModel("sphere/sphere.obj");
	object3D_->SetRotate({ 0,3.14f,0 });
}

void Enemy::Update(){
	BaseCharacter::Update();
}

void Enemy::Draw(){
	object3D_->Draw();
}

void Enemy::OnCollision(Collider* other){
	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeID();
	//衝突相手が敵なら
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kPlayerBullet)) {
		PlayerBullet* playerBullet = static_cast<PlayerBullet*>(other);
		Vector3 distance = playerBullet->GetCenterPosition() - object3D_->GetCenterPosition();
		distance = distance.Normalize() * GetRadius();
		particleSystem_->FindEmitter("hitEffect")->SetPosition(object3D_->GetCenterPosition() + distance);
		particleSystem_->Emit("hitEffect");
	}
}
