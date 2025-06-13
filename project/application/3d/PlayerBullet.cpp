#include "PlayerBullet.h"
#include "TimeManager.h"
#include <CollisionTypeIdDef.h>

void PlayerBullet::Initialize(const Vector3& position, const Vector3& velocity){
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kPlayerBullet));
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize();
	object3d_->SetModel("sphere/sphere.obj");
	object3d_->SetTranslate(position);
	velocity_ = velocity;
	Rotate(velocity_);
}

void PlayerBullet::Update() {
	Rotate(velocity_);
	object3d_->SetTranslate(Vector3::Add(object3d_->GetTranslate(), velocity_));
	object3d_->Update();
	deathTimer_ -= TimeManager::GetInstance()->deltaTime_;
	if (deathTimer_ <= 0.0f) {
		isAlive_ = false;
	}
}

void PlayerBullet::Draw() { object3d_->Draw(); }

void PlayerBullet::OnCollision(Collider* other){
	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeID();
	//衝突相手が敵なら
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kEnemy)) {
		isAlive_ = false;
	}
}

void PlayerBullet::Rotate(const Vector3& velocity) {
	Vector3 rotate = object3d_->GetRotate();
	rotate.y = std::atan2f(velocity.x, velocity.z);
	Vector3 velocityZ = Matrix4x4::Transform(velocity, Matrix4x4::MakeRotateYMatrix(-rotate.y));
	rotate.x = std::atan2f(-velocityZ.y, velocityZ.z);
	object3d_->SetRotate(rotate);
}

Vector3 PlayerBullet::GetCenterPosition() const{
	return object3d_->GetCenterPosition();
}
