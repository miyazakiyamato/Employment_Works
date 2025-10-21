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
	object3d_->Update();
	CameraManager::GetInstance()->SetCamera("RailCamera");
	CameraManager::GetInstance()->FindCamera("RailCamera");
	camera_ = CameraManager::GetInstance()->GetCamera();
	camera_->SetFarClip(600.0f);
	camera_->SetTranslate(offset);
	camera_->Update();
}

void RailCamera::Update() {
	if (controlPoints_.size() >= 2 && preControlPointCount != controlPoints_.size()) {
		pointsDrawing.clear();
		for (size_t i = 0; i < segmentCount + 1; i++) {
			float t = 1.0f / segmentCount * i;
			Vector3 pos = Vector3::CatmullRomPosition(controlPoints_, t);
			pointsDrawing.push_back(pos);
		}
		segmentLength = 0.0f;
		for (size_t i = 0; i < pointsDrawing.size() - 1; i++) {
			segmentLength += (pointsDrawing.at(i) - pointsDrawing.at(i + 1)).Length();
		}
		preControlPointCount = controlPoints_.size();
		isFinished = false;
		segmentPosition = 0.0f;
	}
	segmentPosition += velocity * TimeManager::GetInstance()->deltaTime_;
	//if (segmentPosition > segmentLength) {
	//	// セグメントの場所をリセット
	//	segmentPosition = 0.0f;
	//}
	float t = segmentPosition / segmentLength;
	if (t <= 1.0f) {
		linePosition = Vector3::CatmullRomPosition(controlPoints_, t);
	} else {
		isFinished = true;
	}
	t = (segmentPosition + targetTimeDistance) / segmentLength;
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

	if (controlPoints_.size() < 2) { return; }
	if (pointsDrawing.size() < 2) { return; }
	for (size_t i = 0; i < pointsDrawing.size() - 1; i++) {
		Line3dManager::GetInstance()->DrawLine(pointsDrawing.at(i), pointsDrawing.at(i + 1), { 1.0f,0.0f,0.0f,1.0f });
	}
}

