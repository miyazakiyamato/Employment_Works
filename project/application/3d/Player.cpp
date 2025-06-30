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
	object3d_->SetModel("sphere/sphere.obj");
	object3d_->SetTranslate({ 0.0f, 0.0f, 30.0f });
	//レティクル3D
	reticle3d_ = std::make_unique<Object3d>();
	reticle3d_->Initialize();
	reticle3d_->SetModel("sphere/sphere.obj");
	reticle3d_->SetTranslate({ 0,0,10.0f });
	//レティクル2D
	reticle2d_ = std::make_unique<Sprite>();
	reticle2d_->Initialize("reticle.png");
	reticle2d_->SetPosition({ WinApp::kClientWidth / 2.0f,WinApp::kClientHeight / 2.0f });
	reticle2d_->SetAnchorPoint({ 0.5f, 0.5f });
	reticle2d_->SetSize({ 64.0f, 64.0f });
	//AudioManager::GetInstance()->LoadWave("playerBullet.wav");
	//AudioManager::GetInstance()->LoadWave("playerDamage.wav");
}

void Player::Update(){
	Move();
	CameraMove();
	Attack();

	object3d_->SetTranslate(Vector3::Clamp(object3d_->GetTranslate(), {-50,-50,-100}, {50,50,100}));
	BaseCharacter::Update();
	ReticleUpdate();
}

void Player::Draw(){
	object3d_->Draw();
}

void Player::DrawUi(){
	reticle2d_->Draw();
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
		velocity_.y += moveSpeed_;
	}
	if (input_->PushKey(DIK_S)) {
		velocity_.y -= moveSpeed_;
	}
	if (velocity_.Length() != 0) {
		velocity_.Normalize();
		velocity_ *= moveSpeed_;

		Vector3 position = object3d_->GetTranslate();
		object3d_->SetTranslate(position + velocity_);
	}
	//if (lockOn_ && lockOn_->ExistTarget()) {
	//	// ロックオン座標
	//	Vector3 lockOnPos = lockOn_->GetTargetPosition();
	//	// 追従対象からロックオン対象へのベクトル
	//	Vector3 sub = lockOnPos - object3d_->GetCenterPosition();

	//	float r = 0.05f;
	//	/*if (sub.x <= r && sub.x >= -r || sub.z <= r && sub.z >= -r) {
	//		lockOn_->ResetTarget();
	//	}*/

	//	// Y軸周り角度
	//	Vector3 rotation = object3d_->GetRotate();
	//	rotation.y = std::atan2f(sub.x, sub.z);
	//	object3d_->SetRotate(rotation);
	//}
}

void Player::CameraMove(){
	//// 速さ
	//const float speed = 0.1f;
	//// 移動量
	//Vector3 move{};
	//// 移動
	//Vector3 rotate = camera_->GetRotate();

	//if (lockOn_->ExistTarget()) {
	//	//ロックオン座標
	//	Vector3 lockOnPos = lockOn_->GetTargetPosition();
	//	//追従対象からのロックオン対象へのベクトル
	//	Vector3 sub = lockOnPos - object3d_->GetTranslate();
	//	//Y軸周りの角度
	//	rotate.y = std::atan2(sub.x, sub.z);
	//	destinationAngleY_ = std::atan2(sub.x, sub.z);
	//}
	//else 
	/*{
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
	camera_->SetRotate(rotate);*/
	//追従対象からのオフセット
	//Vector3 offset = { 0.0f, 2.0f, -10.0f };
	////回転行列の合成
	//Matrix4x4 rotateMatrix = Matrix4x4::MakeAffineMatrix({ 1, 1, 1 }, camera_->GetRotate(), {});
	////オフセットをカメラの回転に合わせて回転させる
	////offset = Matrix4x4::TransformNormal(offset, rotateMatrix);
	//offset = Matrix4x4::Transform(offset, rotateMatrix);
	//Vector3 position = object3d_->GetTranslate();
	//camera_->SetTranslate(position + offset);

	//// Y軸周り角度
	//Vector3 rotation = object3d_->GetRotate();
	//rotation.y = camera_->GetRotate().y;
	//object3d_->SetRotate(rotation);
	////object3d_->SetRotate(camera_->GetRotate());
}
void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		velocity = Vector3::Subtract(reticle3d_->GetCenterPosition(), object3d_->GetCenterPosition());
		velocity = Vector3::Multiply(kBulletSpeed, Vector3::Normalize(velocity));

		//velocity = Matrix4x4::TransformNormal(velocity, reticle3D_->GetWorldMatrix());

		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(GetWorldPosition(), velocity);
		gameScene_->AddPlayerBullet(std::move(newBullet));
	}
}

void Player::ReticleUpdate(){
	Vector2 move{};
	float speed = 10.0f;
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
}

Vector3 Player::GetWorldPosition(){
	return object3d_->GetCenterPosition();
}

void Player::SetParent(Object3d* object3d){
	object3d_->SetParent(object3d);
}
