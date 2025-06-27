#include "RailCamera.h"
#include <cmath>
#include "CameraManager.h"
#include "Line3D.h"
#include "TimeManager.h"

void RailCamera::Initialize(const Vector3& position, const Vector3& rotate){
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize();
	offset = position;
	object3d_->SetRotate(rotate);
	object3d_->SetModel("sphere/sphere.obj");
	CameraManager::GetInstance()->SetCamera("RailCamera");
	CameraManager::GetInstance()->FindCamera("RailCamera");
	camera_ = CameraManager::GetInstance()->GetCamera();
	camera_->SetFarClip(600.0f);

	controlPoints_ = {
		{0,  0,  0},
		{0,  0, 10},
		{0,  1, 10},
		{0,  2, 20},
		{0,  1, 20},
		{0,  0, 30},
		{0,  0, 40},
		{0,  0, 50},
		{0,  10, 60},
		{0,  10, 70},
		{0,  20, 80},
		{0,  20, 90},
		{0,  10,100},
		{0,  10,110},
		{0,  0,120},
		{0,  0,130},
		{0,  0,140}
	};

	for (size_t i = 0; i < segmentCount + 1; i++) {
		float t = 1.0f / segmentCount * i;
		Vector3 pos = Vector3::CatmullRomPosition(controlPoints_, t);
		pointsDrawing.push_back(pos);
	}
}

void RailCamera::Update() {
	segmentTime += TimeManager::GetInstance()->deltaTime_;
	if (segmentTime > kSegmentTime) {
		// セグメントの時間をリセット
		segmentTime = 0.0f;
	}
	float t = segmentTime / kSegmentTime;
	if (t <= 1.0f) {
		linePosition = Vector3::CatmullRomPosition(controlPoints_, t);
	}
	t = (segmentTime + targetTimeDistance) / kSegmentTime;
	if (t <= 1.0f) {
		target = Vector3::CatmullRomPosition(controlPoints_, t);
	}

	// 進行方向に見た目の回転を合わせる
	Vector3 rotate = object3d_->GetRotate();
	Vector3 direction = target - linePosition;
	rotate.y = std::atan2f(direction.x, direction.z);
	Vector3 directionZ = Matrix4x4::Transform(direction, Matrix4x4::MakeRotateYMatrix(-rotate.y));
	rotate.x = std::atan2f(-directionZ.y, directionZ.z);
	object3d_->SetRotate(rotate);
	
	// ワールドトランスフォームの座標の数値を加算したりする(移動)
	object3d_->SetTranslate(linePosition + Matrix4x4::Transform(offset, Matrix4x4::MakeAffineMatrix({1,1,1},rotate,{})));

	camera_->SetTranslate(object3d_->GetTranslate());
	camera_->SetRotate(object3d_->GetRotate());
	camera_->Update();
	object3d_->Update();
}

void RailCamera::Draw() {
	object3d_->Draw();
	for (size_t i = 0; i < pointsDrawing.size() - 1; i++) {
		Line3dManager::GetInstance()->DrawLine(pointsDrawing.at(i), pointsDrawing.at(i + 1), { 1.0f,0.0f,0.0f,1.0f });
	}
}

