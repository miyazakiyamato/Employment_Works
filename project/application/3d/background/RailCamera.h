#pragma once
#include "Matrix4x4.h"
#include "Camera.h"
#include "Object3d.h"
#include <vector>

/// <summary>
/// レールカメラ
/// </summary>
class RailCamera {
public:
	//構造体
	struct Shake {
		Vector2 move;
		Vector2 preMove;
		float kTime;
		float time;
		bool isShake = false;
	};
	//メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3& position, const Vector3& rotate);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	/// <summary>
	/// カメラ振動開始
	/// </summary>
	void ShakeStart(Vector2 move, float kTime);
private:
	//カメラ振動
	void Shaking();
	//メンバ変数
	Camera* camera_ = nullptr;
	std::unique_ptr<Object3d> object3d_;

	std::vector<Vector3> controlPoints_;
	std::vector<Vector3> pointsDrawing;
	size_t segmentCount = 100;
	size_t preControlPointCount = 0;

	Vector3 linePosition;
	Vector3 target;
	Vector3 offset = { 0.0f, 2.0f, -10.0f }; // カメラのオフセット

	float segmentLength = 0.0f; // セグメントの長さ
	float velocity = 10.0f;		// セグメントの速度
	float segmentPosition = 0.0f; // セグメントの位置
	float targetTimeDistance = 0.5f; // ターゲットまでの時間間隔

	Shake shake_{};

	bool isFinished = false;
public:
	const Camera* GetCamera() const { return camera_; }
	Object3d* GetObject3d() { return object3d_.get(); }
	bool GetIsFinished() const { return isFinished; }
	std::vector<Vector3> GetControlPoints() const { return controlPoints_; }

	void SetControlPoints(const std::vector<Vector3>& points) { controlPoints_ = points; }
	void AddControlPoint(const Vector3& point) { controlPoints_.push_back(point); }
	void SetVelocity(float v) { velocity = v; }
	void SetSegmentCount(size_t count) { segmentCount = count;}
};
