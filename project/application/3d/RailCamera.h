#pragma once
#include "Matrix4x4.h"
#include "Camera.h"
#include "Object3d.h"
#include <vector>

class RailCamera {
public:
	void Initialize(const Vector3& position, const Vector3& rotate);
	void Update();
	void Draw();

private:
	Camera* camera_ = nullptr;
	std::unique_ptr<Object3d> object3d_;

	std::vector<Vector3> controlPoints_;
	std::vector<Vector3> pointsDrawing;
	const size_t segmentCount = 100;

	Vector3 linePosition;
	Vector3 target;
	Vector3 offset = { 0.0f, 2.0f, -10.0f }; // カメラのオフセット

	const float kSegmentTime = 20.0f; // セグメントの時間間隔
	float segmentTime = 0;
	float targetTimeDistance = 0.01f; // ターゲットまでの時間間隔
public:
	const Camera* GetCamera() const { return camera_; }
	Object3d* GetObject3d() { return object3d_.get(); }
};
