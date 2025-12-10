#include "Enemy.h"
#include <CollisionTypeIdDef.h>
#include "ParticleSystem.h"
#include "PlayerBullet.h"
#include "BulletManager.h"
#include "EnemyBullet.h"
#include "Player.h"
#include <cassert>
#include "EmitterSphere.h"
#include <TimeManager.h>

void Enemy::Initialize(){
	BaseCharacter::Initialize();
	Collider::Initialize();
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kEnemy));
	Collider::SetRadius(1.0f);
	object3d_->SetTranslate({ 0.0f, 1.0f, 30.0f });
	object3d_->SetModel("drone/drone.obj");
	object3d_->SetRotate({ 0,3.14f,0 });
	object3d_->SetColor({ 0.0f, 0.0f, 0.0f, 1.0f },0);
	object3d_->Update();
	FireTimed();
}

void Enemy::Update() {
	if (dethTimer_ > 0) {
		dethTimer_ += TimeManager::deltaTime_;
		object3d_->SetTranslate(object3d_->GetTranslate() + Vector3(0, -10, 0) * TimeManager::deltaTime_);
		if (dethTimer_ >= kDethTimer) {
			isAlive_ = false;
		}
	}
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
	Shaking();
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
		Damage(int(playerBullet->GetObject3d()->GetScale().x * 4.0f), distance.Normalize());
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

void Enemy::Damage(int damage, const Vector3& hitDirection) {
	static_cast<EmitterSphere*>(particleSystem_->FindEmitter("hitEffect"))->SetTranslate(object3d_->GetCenterPosition() + hitDirection * GetRadius());
	particleSystem_->Emit("hitEffect");
	DamageKnockbackStart(
		hitDirection,     // ダメージ方向
		30.0f,      // どれくらい倒すか（度数）
		0.3f       // 戻るまでの時間
	);
	hp_ -= damage;
	if (hp_ <= 0) {
		dethTimer_ += TimeManager::deltaTime_;
		hp_ = 0;
	}
}

void Enemy::ShakeStart(Quaternion shakeQuaternion, float kTime) {
	shake_.shakeQuaternion = shakeQuaternion;
	shake_.preShakeQuaternion = {};
	shake_.kTime = kTime;
	shake_.time = 0.0f;
	shake_.isShake = true;
}
void Enemy::DamageKnockbackStart(const Vector3& hitDirection, float power, float duration)
{
	// 現在の回転を保存
	if (!shake_.isShake) {
		shake_.startQuaternion = object3d_->GetRotate().ToQuaternion();
	}
	// ダメージ方向に倒す回転
	Vector3 dir = hitDirection.Normalize();

	// 回転軸＝ダメージ方向と「上方向」の外積
	Vector3 axis = Vector3::Cross(dir, { 0,1,0 });
	if (axis.Length() < 0.0001f) axis = { 1,0,0 }; // 真上/真下対策
	axis.Normalize();

	// power（何度傾けるか）をラジアンに変換
	float rad = power * (3.14159f / 180.0f);

	Quaternion damageRot = Quaternion::MakeRotateAxisAngle(axis, rad).ToQuaternion();

	// 最終的に倒れるQuaternion = 現在 * damageRot
	Quaternion target = shake_.startQuaternion * damageRot;

	// Shake 開始
	ShakeStart(target, duration);
}
void Enemy::Shaking() {
	if (!shake_.isShake) { return; }

	shake_.time += TimeManager::deltaTime_;
	float t = shake_.time / shake_.kTime;

	if (t >= 1.0f) {
		// 回転を元に戻す
		object3d_->SetRotate(shake_.startQuaternion.ToEulerAngles());
		shake_.isShake = false;
		return;
	}

	// 線形補間で徐々に倒れ → 元に戻る
	float ease = (t < 0.5f)
		? (t * 2.0f)          // 0 → 1（倒れる）
		: (1.0f - (t - 0.5f) * 2.0f); // 1 → 0（戻る）

	Quaternion q = Quaternion::Slerp(
		shake_.startQuaternion,
		shake_.shakeQuaternion,
		ease
	);

	object3d_->SetRotate(q.ToEulerAngles());
}


void Enemy::Fire() {
	assert(player_);
	//
	const float kBulletSpeed = 30.0f;

	Vector3 velocity{ player_->GetWorldPosition() - object3d_->GetCenterPosition() };
	velocity = Vector3::Multiply(kBulletSpeed, velocity.Normalize());

	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(object3d_->GetCenterPosition(), velocity);
	bulletManager_->AddBullet(std::move(newBullet));
}
