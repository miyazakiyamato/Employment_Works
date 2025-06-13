#pragma once
#include <memory>
#include "Matrix4x4.h"
#include "Object3d.h"
#include "Collider.h"

class PlayerBullet : public Collider{
public:
	~PlayerBullet() {}
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3& position,const Vector3& velocity);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void OnCollision([[maybe_unused]] Collider* other) override;

private:
	// 寿命
	static inline const float kLifeTime_ = 5.0f;

	Vector3 velocity_{};
	// モデル
	std::unique_ptr<Object3d> object3d_ = nullptr;
	//デスタイマー
	float deathTimer_ = kLifeTime_;
	//生存フラグ
	bool isAlive_ = true;
	//
	void Rotate(const Vector3& velocity);

public:
	bool isAlive() const { return isAlive_; }
	Vector3 GetCenterPosition()const override;
};
