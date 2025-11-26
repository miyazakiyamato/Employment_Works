#include "Player.h"
#include "Input.h"
#include "CameraManager.h"
#include <algorithm>
#include "BulletManager.h"
#include "AudioManager.h"
#include "CollisionTypeIdDef.h"
#include "PlayerBullet.h"
#include "ParticleSystem.h"
#include "EmitterSphere.h"
#include "EnemyBullet.h"
#include "RailCamera.h"
#include "TimeManager.h"

void Player::Initialize(){
	BaseCharacter::Initialize();
	Collider::Initialize();
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kPlayer));
	Collider::SetRadius(1.0f);
	input_ = Input::GetInstance();
	object3d_->SetModel("airship/airship.obj");
	object3d_->SetTranslate({ 0.0f, 0.0f, 30.0f });
	object3d_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f }, 0);
	object3d_->Update();
	//レティクル3D
	reticle3d_ = std::make_unique<Object3d>();
	reticle3d_->Initialize();
	reticle3d_->SetModel("sphere/sphere.obj");
	reticle3d_->SetTranslate({ 0,0,10.0f });
	reticle3d_->Update();
	//レティクル2D
	reticle2d_ = std::make_unique<Sprite>();
	reticle2d_->Initialize("reticle.png");
	reticle2d_->SetPosition({ WinApp::kClientWidth / 2.0f,WinApp::kClientHeight / 2.0f });
	reticle2d_->SetAnchorPoint({ 0.5f, 0.5f });
	reticle2d_->SetSize({ 64.0f, 64.0f });
	reticle2d_->Update();
}

void Player::Update(){
	Move();
	object3d_->SetTranslate(Vector3::Clamp(object3d_->GetTranslate(), {-50,-50,-100}, {50,50,100}));
	
	if (attackData_.isCharging == false) {
		Attack();
	} else {
		if (attackData_.chargeCount < attackData_.kChargeTime) {
			Shoot();
		} else {
			ChargeShoot();
		}
	}

	static_cast<EmitterSphere*>(particleSystem_->FindEmitter("airEffect"))->SetTranslate(object3d_->GetCenterPosition());

	BaseCharacter::Update();
	ReticleUpdate();
}

void Player::ClearUpdate(){

	// 回転スピード
	float revolveSpeed = 1.0f * 3.141592f / 180.0f; // 1度/フレーム
	float rotateSpeed = revolveSpeed;             // 1周ごとに1回転 → 公転と同じ増え方

	
	// 公転
	revolveAngle_ += revolveSpeed;

	// 半径
	float radius = 5.0f;

	// 位置 = 原点から見た円運動
	Vector3 newPos = {
		radius * std::sin(revolveAngle_) + 8.0f,
		1.0f,
		radius* std::cos(revolveAngle_) + 30.0f
	};
	Vector3 nextPos = {
		radius * std::sin(revolveAngle_ + revolveSpeed) + 8.0f,
		1.0f,
		radius * std::cos(revolveAngle_ + revolveSpeed) + 30.0f
	};

	// プレイヤーの位置を更新
	object3d_->SetTranslate(newPos);
	// 向きの更新
	Vector3 direction = Vector3::Subtract(nextPos, newPos);
	Vector3 rotate = object3d_->GetRotate();
	rotate.y = std::atan2f(direction.x, direction.z);
	Vector3 velocityZ = Matrix4x4::Transform(direction, Matrix4x4::MakeRotateYMatrix(-rotate.y));
	rotate.x = std::atan2f(-velocityZ.y, velocityZ.z);
	object3d_->SetRotate(rotate);

	BaseCharacter::Update();
}

void Player::LeaveUpdate(){
	object3d_->SetTranslate(object3d_->GetTranslate() + Vector3(0.0f,0.0f,1.0f));
	BaseCharacter::Update();
}

void Player::Draw(){
	object3d_->Draw();
}

void Player::DrawUi(){
	reticle2d_->Draw();
}

void Player::OnCollision([[maybe_unused]] Collider* other){
	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeID();
	//衝突相手が敵なら
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kEnemyBullet)) {
		EnemyBullet* enemyBullet = static_cast<EnemyBullet*>(other);
		Vector3 distance = enemyBullet->GetCenterPosition() - object3d_->GetCenterPosition();
		distance = distance.Normalize() * GetRadius();
		static_cast<EmitterSphere*>(particleSystem_->FindEmitter("hitEffect"))->SetTranslate(object3d_->GetCenterPosition() + distance);
		particleSystem_->Emit("hitEffect");
		railCamera_->ShakeStart({ 5.0f + velocity_.Length() / 10.0f,5.0f + velocity_.Length() / 10.0f }, 0.3f);
		hp_ -= 1;
		if (hp_ <= 0) {
			isAlive_ = false;
			hp_ = 0;
		}
	}
	
}

void Player::Move(){// 移動量
	velocity_ = { 0,0,0 };

	velocity_.x += input_->GetControllerStickLX();
	velocity_.y += input_->GetControllerStickLY();
	if (velocity_.Length() == 0) {
		if (input_->PushKey(DIK_A)) {
			velocity_.x -= moveSpeed_;
		}
		if (input_->PushKey(DIK_D)) {
			velocity_.x += moveSpeed_;
		}
		if (input_->PushKey(DIK_W)) {
			velocity_.y += moveSpeed_;
		}
		if (input_->PushKey(DIK_S)) {
			velocity_.y -= moveSpeed_;
		}
	}
	if (input_->PushKey(DIK_LSHIFT) || input_->PushControllerButton(XINPUT_GAMEPAD_LEFT_SHOULDER)) {
		railCamera_->SetVelocity(30.0f);
	} else {
		railCamera_->SetVelocity(15.0f);
	}

	if (velocity_.Length() != 0) {
		velocity_.Normalize();
		velocity_ *= moveSpeed_ * TimeManager::GetInstance()->deltaTime_;

		Vector3 position = object3d_->GetTranslate();
		object3d_->SetTranslate(position + velocity_);
	}
}

void Player::Attack() {
	if (input_->PushKey(DIK_SPACE) || input_->PushControllerButton(XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
		attackData_.chargeCount += TimeManager::GetInstance()->deltaTime_;
		if (attackData_.chargeCount >= attackData_.kChargeTime) {
			attackData_.isCharging = true;
		}
	} else {
		if (attackData_.chargeCount != 0.0f) {
			attackData_.isCharging = true;
		}
		attackData_.chargeCount = 0.0f;
	}
}

void Player::ReticleUpdate(){
	Vector2 move{};
	float speed = 10.0f;

	move.x += input_->GetControllerStickRX();
	move.y -= input_->GetControllerStickRY();
	if (move.Length() == 0) {
		if (Input::GetInstance()->PushKey(DIK_LEFT)) {
			move += { -1.0f, 0.0f};
		}
		if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
			move += { 1.0f, 0.0f};
		}
		if (Input::GetInstance()->PushKey(DIK_UP)) {
			move += { 0.0f, -1.0f };
		}
		if (Input::GetInstance()->PushKey(DIK_DOWN)) {
			move += { 0.0f, 1.0f};
		}
	}
	if (move.Length() != 0) {
		move.Normalize();
		move *= speed;
		Vector2 spritePosition = reticle2d_->GetPosition();
		spritePosition += move;
		spritePosition.x = std::clamp(spritePosition.x, 0.0f, (float)WinApp::kClientWidth);
		spritePosition.y = std::clamp(spritePosition.y, 0.0f, (float)WinApp::kClientHeight);
		reticle2d_->SetPosition(spritePosition);
	}
	//
	Matrix4x4 matViewport = Matrix4x4::MakeViewportMatrix(0, 0, WinApp::kClientWidth, WinApp::kClientHeight, 0, 1);
	Matrix4x4 matVPV = Matrix4x4(camera_->GetViewMatrix()) * camera_->GetProjectionMatrix() * matViewport;
	Matrix4x4 matInverseVPV = Matrix4x4::Inverse(matVPV);
	Vector3 posNear = Vector3(reticle2d_->GetPosition().x, reticle2d_->GetPosition().y, 0);
	Vector3 posFar = Vector3(reticle2d_->GetPosition().x, reticle2d_->GetPosition().y, 1);
	posNear = Matrix4x4::Transform(posNear, matInverseVPV);
	posFar = Matrix4x4::Transform(posFar, matInverseVPV);
	Vector3 mouseDirection = Vector3::Subtract(posFar, posNear);
	mouseDirection = Vector3::Normalize(mouseDirection);
	//
	const float kDistanceTestObject = 100.0f;
	reticle3d_->SetTranslate(Vector3::Add(posNear, Vector3::Multiply(kDistanceTestObject, mouseDirection)));
	reticle3d_->Update();
	reticle2d_->Update();
	Vector3 direction = Vector3::Subtract(reticle3d_->GetCenterPosition(), object3d_->GetCenterPosition());
	Vector3 rotate = object3d_->GetRotate();
	rotate.y = std::atan2f(direction.x, direction.z);
	Vector3 velocityZ = Matrix4x4::Transform(direction, Matrix4x4::MakeRotateYMatrix(-rotate.y));
	rotate.x = std::atan2f(-velocityZ.y, velocityZ.z);
	object3d_->SetRotate(rotate);
}

void Player::Shoot(){
	attackData_.chargeCount += TimeManager::GetInstance()->deltaTime_;
	if (attackData_.bulletCount == 0 ||
		attackData_.chargeCount >= attackData_.kChargeTime) {
		// 弾の速度
		Vector3 velocity(0, 0, attackData_.kBulletSpeed);
		velocity = Vector3::Subtract(reticle3d_->GetCenterPosition(), object3d_->GetCenterPosition());
		velocity = Vector3::Multiply(attackData_.kBulletSpeed, Vector3::Normalize(velocity));
		// 弾の生成
		std::unique_ptr<BaseBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(GetWorldPosition(), velocity);
		newBullet->GetObject3d()->SetScale({ attackData_.kChargeTime,attackData_.kChargeTime,attackData_.kChargeTime });
		bulletManager_->AddBullet(std::move(newBullet));
		
		// 次の弾への初期化
		attackData_.bulletCount++;
		attackData_.chargeCount = 0.0f;
	}
	if (attackData_.bulletCount >= attackData_.kBulletCount) {
		attackData_.isCharging = false;
		attackData_.bulletCount = 0;
	}
}

void Player::ChargeShoot(){
	EmitterSphere* chargeEffect = static_cast<EmitterSphere*>(particleSystem_->FindEmitter("chargeEffect"));
	chargeEffect->SetTranslate(object3d_->GetCenterPosition());
	chargeEffect->SetIsEmitUpdate(true);
	attackData_.chargeCount += TimeManager::GetInstance()->deltaTime_;
	if (!input_->PushKey(DIK_SPACE) && !input_->PushControllerButton(XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
		// 弾の速度
		Vector3 velocity(0, 0, attackData_.kBulletSpeed);
		velocity = Vector3::Subtract(reticle3d_->GetCenterPosition(), object3d_->GetCenterPosition());
		velocity = Vector3::Multiply(attackData_.kBulletSpeed, Vector3::Normalize(velocity));
		// 弾の生成
		std::unique_ptr<BaseBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(GetWorldPosition(), velocity);
		newBullet->GetObject3d()->SetScale({ attackData_.chargeCount,attackData_.chargeCount,attackData_.chargeCount });
		bulletManager_->AddBullet(std::move(newBullet));

		// 次の弾への初期化
		attackData_.isCharging = false;
		attackData_.chargeCount = 0.0f;
		chargeEffect->SetIsEmitUpdate(false);
	}
}

Vector3 Player::GetWorldPosition(){
	return object3d_->GetCenterPosition();
}

void Player::SetParent(Object3d* object3d){
	object3d_->SetParent(object3d);
}
