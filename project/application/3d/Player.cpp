#include "Player.h"
#include "Input.h"
#include "CameraManager.h"
#include <algorithm>
#include "GameScene.h"
#include "AudioManager.h"
#include "CollisionTypeIdDef.h"
#include "PlayerBullet.h"

void Player::Initialize(){
	BaseCharacter::Initialize();
	Collider::Initialize();
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kPlayer));
	Collider::SetRadius(4.0f);
	input_ = Input::GetInstance();
	camera_ = CameraManager::GetInstance()->GetCamera();
	object3D_->SetTranslate({ -1.0f, 1.0f, -30.0f });
	model_ = std::make_unique<Object3d>();
	model_->Initialize();
	model_->SetParent(object3D_.get());
	model_->SetModel("sphere/sphere.obj");
	model_->SetRotate({ 0,3.14f,0 });
	//レティクル3D
	reticle3D_ = std::make_unique<Object3d>();
	reticle3D_->Initialize();
	reticle3D_->SetModel("sphere/sphere.obj");
	reticle3D_->SetTranslate({ 0,0,10.0f });
	reticle3D_->SetParent(object3D_.get());
	//レティクル2D
	reticle2D_ = std::make_unique<Sprite>();
	reticle2D_->Initialize("circle2.png");
	reticle2D_->SetPosition({ 0,0 });
	reticle2D_->SetSize({ 100.0f,100.0f });
	//AudioManager::GetInstance()->LoadWave("playerBullet.wav");
	//AudioManager::GetInstance()->LoadWave("playerDamage.wav");
}

void Player::Update(){
	BaseCharacter::Update();
	Move();
	CameraMove();
	Attack();

	object3D_->SetTranslate(Vector3::Clamp(object3D_->GetTranslate(), {-100,0,-100}, {100,50,100}));
	reticle3D_->Update();
	model_->Update();
}

void Player::Draw(){
	model_->Draw();
	//reticle3D_->Draw();
}

void Player::OnCollision([[maybe_unused]] Collider* other){
	isCollision = true;

	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeID();
	//衝突相手が敵なら
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kEnemy)) {
		
	}
	
}

void Player::Move(){// 移動量
	velocity_ = { 0,0,0 };
	if (input_->PushKey(DIK_A)) {
		velocity_.x -= moveSpeed_;
	}
	if (input_->PushKey(DIK_D)) {
		velocity_.x += moveSpeed_;
	}
	if (input_->PushKey(DIK_W)) {
		velocity_.z += moveSpeed_;
	}
	if (input_->PushKey(DIK_S)) {
		velocity_.z -= moveSpeed_;
	}
	if (velocity_.Length() != 0) {
		velocity_.Normalize();
		velocity_ *= moveSpeed_;

		Matrix4x4 rotateMatrix = Matrix4x4::MakeRotateYMatrix(camera_->GetRotate().y);
		velocity_ = Matrix4x4::Transform(velocity_, rotateMatrix);

		Vector3 rotation{};
		rotation.y = std::atan2f(velocity_.x, velocity_.z);
		Vector3 velocityZ = Matrix4x4::Transform(velocity_, Matrix4x4::MakeRotateYMatrix(-rotation.y));
		rotation.x = std::atan2f(-velocityZ.y, velocityZ.z);

		Vector3 position = object3D_->GetTranslate();
		object3D_->SetTranslate(position + velocity_);
		object3D_->SetRotate(rotation);
	}
	//if (lockOn_ && lockOn_->ExistTarget()) {
	//	// ロックオン座標
	//	Vector3 lockOnPos = lockOn_->GetTargetPosition();
	//	// 追従対象からロックオン対象へのベクトル
	//	Vector3 sub = lockOnPos - object3D_->GetCenterPosition();

	//	float r = 0.05f;
	//	/*if (sub.x <= r && sub.x >= -r || sub.z <= r && sub.z >= -r) {
	//		lockOn_->ResetTarget();
	//	}*/

	//	// Y軸周り角度
	//	Vector3 rotation = object3D_->GetRotate();
	//	rotation.y = std::atan2f(sub.x, sub.z);
	//	object3D_->SetRotate(rotation);
	//}
	/*if (input_->TriggerKey(DIK_SPACE)) {
		AudioManager::GetInstance()->PlayWave("playerBullet.wav", 1.25f);
		float kBulletSpeed = 0.5f;
		Vector3 velocity(0, 0, kBulletSpeed);

		velocity = reticle3D_->GetCenterPosition() - model_->GetCenterPosition();
		velocity = velocity.Normalize() * kBulletSpeed;

		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(object3D_->GetCenterPosition() + Vector3(velocity * 10.0f));
		newBullet->SetVelocity(velocity);
		newBullet->SetRotate(camera_->GetRotate());
		gameScene_->AddPlayerBullet(std::move(newBullet));
	}*/
}

void Player::CameraMove(){
	// 速さ
	const float speed = 0.1f;
	// 移動量
	Vector3 move{};
	// 移動
	Vector3 rotate = camera_->GetRotate();

	//if (lockOn_->ExistTarget()) {
	//	//ロックオン座標
	//	Vector3 lockOnPos = lockOn_->GetTargetPosition();
	//	//追従対象からのロックオン対象へのベクトル
	//	Vector3 sub = lockOnPos - object3D_->GetTranslate();
	//	//Y軸周りの角度
	//	rotate.y = std::atan2(sub.x, sub.z);
	//	destinationAngleY_ = std::atan2(sub.x, sub.z);
	//}
	//else 
	{
		if (Input::GetInstance()->PushKey(DIK_LEFT)) {
			move += { 0.0f, -1.0f, 0.0f };
		}
		if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
			move += { 0.0f, 1.0f, 0.0f };
		}
		if (Input::GetInstance()->PushKey(DIK_UP)) {
			move += { -1.0f, 0.0f, 0.0f };
		}
		if (Input::GetInstance()->PushKey(DIK_DOWN)) {
			move += { 1.0f, 0.0f, 0.0f };
		}

		if (move.Length() != 0) {
			move.Normalize();
			move *= speed;

			destinationAngleX_ = std::clamp(rotate.x + move.x, -1.5f, 1.5f);
			destinationAngleY_ = rotate.y + move.y;
		}
	}
	rotate.x = rotate.x + 0.1f * (destinationAngleX_ - rotate.x);
	rotate.y = rotate.y + 0.1f * (destinationAngleY_ - rotate.y);
	camera_->SetRotate(rotate);
	//追従対象からのオフセット
	Vector3 offset = { 0.0f, 2.0f, -10.0f };
	//回転行列の合成
	Matrix4x4 rotateMatrix = Matrix4x4::MakeAffineMatrix({ 1, 1, 1 }, camera_->GetRotate(), {});
	//オフセットをカメラの回転に合わせて回転させる
	//offset = Matrix4x4::TransformNormal(offset, rotateMatrix);
	offset = Matrix4x4::Transform(offset, rotateMatrix);
	Vector3 position = object3D_->GetTranslate();
	camera_->SetTranslate(position + offset);

	// Y軸周り角度
	Vector3 rotation = object3D_->GetRotate();
	rotation.y = camera_->GetRotate().y;
	object3D_->SetRotate(rotation);
	//object3D_->SetRotate(camera_->GetRotate());
}
void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		velocity = Vector3::Subtract(reticle3D_->GetCenterPosition(), object3D_->GetCenterPosition());
		velocity = Vector3::Multiply(kBulletSpeed, Vector3::Normalize(velocity));

		//velocity = Matrix4x4::TransformNormal(velocity, reticle3D_->GetWorldMatrix());

		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(GetWorldPosition(), velocity);
		gameScene_->AddPlayerBullet(std::move(newBullet));
	}
}

Vector3 Player::GetWorldPosition(){
	return object3D_->GetCenterPosition();
}
