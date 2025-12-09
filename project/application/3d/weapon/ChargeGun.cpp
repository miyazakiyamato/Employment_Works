#include "ChargeGun.h"
#include "Player.h"
#include "Input.h"
#include "TimeManager.h"
#include "PlayerStateShoot.h"
#include "PlayerStateChargeShoot.h"

// 初期化
void ChargeGun::Initialize(){
	BaseWeapon::Initialize();
	input_ = Input::GetInstance();
	timeManager_ = TimeManager::GetInstance();

	object3d_->SetModel("chargeGun/chargeGun.obj");
	object3d_->SetScale({ 0.8f,0.8f,0.8f });
	gunBarrel_->SetTranslate({ 0.0f,0.0f,4.0f });
	gunBarrel_->SetParent(object3d_.get());
	// 攻撃データの設定
	AttackData& rootAttackData = attackDatas_[static_cast<uint32_t>(AttackType::kRoot)];
	rootAttackData.kBulletSpeed = 150.0f;
	rootAttackData.bulletSize = 0.3f;
	rootAttackData.kCoolTime = 0.2f;
	rootAttackData.kBulletCount = 3;
	AttackData& chargeAttackData = attackDatas_[static_cast<uint32_t>(AttackType::kCharge)];
	chargeAttackData.kBulletSpeed = 100.0f;
	chargeAttackData.bulletSize = 0.3f;
	chargeAttackData.kCoolTime = 0.5f;
	chargeAttackData.kBulletCount = 1;
}

// 更新
void ChargeGun::Update(){
	// 銃の向きを更新
	Vector3 targetPosition = object3d_->GetCenterPosition() + Vector3(0.0f, 0.0f, 1.0f);
	if (target_) {
		targetPosition = target_->GetCenterPosition();
	}
	Vector3 direction = Vector3::Subtract(targetPosition, object3d_->GetCenterPosition()).Normalize();
	Quaternion targetQuaternion = Quaternion::DirectionToDirection({ 0.0f, 0.0f, 1.0f }, direction).ToQuaternion();
	Quaternion parentQuaternion = object3d_->GetParent()->GetWorldMatrix().ToQuaternion();
	Quaternion quaternion = Quaternion::Inverse(parentQuaternion) * targetQuaternion;

	object3d_->SetRotate(quaternion.ToEulerAngles());
	BaseWeapon::Update();
}

// 描画
void ChargeGun::Draw(){
	BaseWeapon::Draw();
}

// 衝突検知時の処理
void ChargeGun::OnCollision([[maybe_unused]] Collider* other){

}

void ChargeGun::Charge(){
	BaseWeapon::Charge();
	AttackData& attackData = attackDatas_[static_cast<uint32_t>(AttackType::kCharge)];
	attackData.bulletSize = std::clamp(chargeCount_, 0.0f, 10.0f);
}
