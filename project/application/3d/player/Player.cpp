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
#include "PlayerStateRoot.h"
#include "ChargeGun.h"

void Player::Initialize(){
	BaseCharacter::Initialize();
	Collider::Initialize();
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kPlayer));
	Collider::SetRadius(1.0f);
	input_ = Input::GetInstance();
	object3d_->SetModel("drone/drone.obj");
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

	//手
	hand_ = std::make_unique<Object3d>();
	hand_->Initialize();
	hand_->SetTranslate(handOffset_);
	hand_->SetParent(object3d_.get());

	//プレイヤーの初期行動ステート
	ChangeState(std::make_unique<PlayerStateRoot>(this));
}

void Player::Update(){
	state_->Update();
	BaseCharacter::Update();
	hand_->Update();
	if (weapon_) {
		weapon_->Update();
	}
}

void Player::Draw(){
	object3d_->Draw();
	if (weapon_) {
		weapon_->Draw();
	}
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

	object3d_->SetTranslate(Vector3::Clamp(object3d_->GetTranslate(), -moveLimit_, moveLimit_));
	
	static_cast<EmitterSphere*>(particleSystem_->FindEmitter("airEffect"))->SetTranslate(object3d_->GetCenterPosition());
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
	//マウスの位置から3Dレティクルの位置を計算
	Matrix4x4 matViewport = Matrix4x4::MakeViewportMatrix(0, 0, WinApp::kClientWidth, WinApp::kClientHeight, 0, 1);
	Matrix4x4 matVPV = Matrix4x4(camera_->GetViewMatrix()) * camera_->GetProjectionMatrix() * matViewport;
	Matrix4x4 matInverseVPV = Matrix4x4::Inverse(matVPV);
	Vector3 posNear = Vector3(reticle2d_->GetPosition().x, reticle2d_->GetPosition().y, 0);
	Vector3 posFar = Vector3(reticle2d_->GetPosition().x, reticle2d_->GetPosition().y, 1);
	posNear = Matrix4x4::Transform(posNear, matInverseVPV);
	posFar = Matrix4x4::Transform(posFar, matInverseVPV);
	Vector3 mouseDirection = Vector3::Subtract(posFar, posNear);
	mouseDirection = Vector3::Normalize(mouseDirection);
	
	//　3Dレティクルの位置を更新
	const float kDistanceTestObject = 100.0f;
	reticle3d_->SetTranslate(Vector3::Add(posNear, Vector3::Multiply(kDistanceTestObject, mouseDirection)));
	reticle3d_->Update();
	reticle2d_->Update();
}

Vector3 Player::GetWorldPosition(){
	return object3d_->GetCenterPosition();
}

void Player::SetParent(Object3d* object3d){
	object3d_->SetParent(object3d);
}

void Player::SetWeapon(std::unique_ptr<BaseWeapon> weapon){
	weapon_ = std::move(weapon);
	weapon_->GetObject3d()->SetParent(hand_.get());
	weapon_->SetTarget(reticle3d_.get());
}