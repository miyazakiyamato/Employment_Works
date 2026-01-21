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
	//object3d_->SetModel("sphere/sphere.obj");
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
		t = (segmentPosition + targetTimeDistance) / segmentLength;
		target = Vector3::CatmullRomPosition(controlPoints_, t);
	} else {
		isFinished = true;
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

	// カメラ振動
	Shaking();
	camera_->SetTranslate(object3d_->GetTranslate());
	camera_->SetRotate(object3d_->GetRotate());
	camera_->Update();
	object3d_->Update();
}

void RailCamera::Draw() {
	object3d_->Draw();
	
#ifdef _DEBUG
	if (controlPoints_.size() < 2) { return; }
	if (pointsDrawing.size() < 2) { return; }
	for (size_t i = 0; i < pointsDrawing.size() - 1; i++) {
		Line3dManager::GetInstance()->DrawLine(pointsDrawing.at(i), pointsDrawing.at(i + 1), { 1.0f,0.0f,0.0f,1.0f });
	}
#endif // _DEBUG
}

void RailCamera::ShakeStart(Vector2 move, float kTime){
	shake_.move = move;
	shake_.preMove = {};
	shake_.kTime = kTime;
	shake_.time = 0.0f;
	shake_.isShake = true;
}

void RailCamera::Shaking(){
	if (!shake_.isShake) {
		return;
	}
	shake_.time += TimeManager::deltaTime_;
	object3d_->SetTranslate(object3d_->GetTranslate() - Vector3(shake_.preMove.x, shake_.preMove.y, 0));
	float timeCount = shake_.time / shake_.kTime;
	if (timeCount > 1.0f) {
		shake_.isShake = false;
		return;
	}
	static std::random_device rd;
	static std::mt19937 gen(rd());
	float shakeSize = 0;
	if (timeCount <= 0.5f) {
		shakeSize = timeCount * 2.0f;
	} else {
		shakeSize = 1.0f - (timeCount * 2.0f - 0.5f);
	}
	static std::uniform_real_distribution<float> distX(-shake_.move.x * shakeSize, shake_.move.x * shakeSize);
	static std::uniform_real_distribution<float> distY(-shake_.move.y * shakeSize, shake_.move.y * shakeSize);

	shake_.preMove = { distX(gen), distY(gen) };
	object3d_->SetTranslate(object3d_->GetTranslate() + Vector3(shake_.preMove.x, shake_.preMove.y, 0));
}

