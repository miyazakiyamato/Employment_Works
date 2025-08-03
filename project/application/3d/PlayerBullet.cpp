#include "PlayerBullet.h"
#include "TimeManager.h"
#include <CollisionTypeIdDef.h>

void PlayerBullet::Initialize(const Vector3& position, const Vector3& velocity){
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kPlayerBullet));
	BaseBullet::Initialize(position, velocity);
	object3d_->SetModel("airship/airship.obj");
	object3d_->SetScale({ 0.5f, 0.5f, 0.5f });
	object3d_->SetColor({ 0.2f, 0.2f, 0.2f, 1.0f },0);
	Rotate(velocity_);
}

void PlayerBullet::Update() {
	BaseBullet::Update();
}

void PlayerBullet::Draw() { 
	BaseBullet::Draw();
}

void PlayerBullet::OnCollision(Collider* other){
	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeID();
	//衝突相手が敵なら
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kEnemy)) {
		isAlive_ = false;
	}
}