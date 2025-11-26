#pragma once
#include "BaseCharacter.h"
//#include "LockOn.h"
#include "ContactRecord.h"
#include "Sprite.h"

class Input;
class Camera;
class BulletManager;
class ParticleSystem;
class RailCamera;
class Player : public BaseCharacter{
public://構造体
	struct AttackData{
		bool isCharging = false;
		float kBulletSpeed = 100.0f;
		float kChargeTime = 0.3f;
		float chargeCount = 0.0f;
		uint32_t kBulletCount = 3;
		uint32_t bulletCount = 0;
		//BaseBullet* bullet = nullptr;
	};

public://メンバ関数
	Player() {}

	~Player() {}
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;
	void ClearUpdate();
	void LeaveUpdate();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;
	void DrawUi();
	//衝突を検知したら呼び出されるコールバック関数
	void OnCollision([[maybe_unused]] Collider* other) override;
private://ローカル関数
	void Move();
	void Attack();
	void ReticleUpdate();
	void Shoot();
	void ChargeShoot();
private://メンバ変数
	Input* input_ = nullptr;
	const Camera* camera_ = nullptr;
	BulletManager* bulletManager_ = nullptr;
	ParticleSystem* particleSystem_ = nullptr;
	RailCamera* railCamera_ = nullptr;
	//3Dレティクル
	std::unique_ptr<Object3d> reticle3d_ = nullptr;
	std::unique_ptr<Sprite> reticle2d_ = nullptr;

	Vector3 velocity_{};
	float moveSpeed_ = 3.0f;
	// clear毎フレーム更新する角度（公転）
	float revolveAngle_ = 0.0f;

	//攻撃データ
	AttackData attackData_{};

	int hp_ = 10;
	bool isAlive_ = true;
	//ロックオン
	//LockOn* lockOn_ = nullptr;
public://ゲッターセッター
	Vector3 GetWorldPosition();
	int GetHp() { return hp_; }
	bool GetIsAlive() const { return isAlive_; }

	//void SetLockOn(LockOn* lockOn) { lockOn_ = lockOn; }
	void SetBulletManager(BulletManager* bulletManager) { bulletManager_ = bulletManager; }
	void SetParent(Object3d* object3d);
	void SetCamera(const Camera* camera) { camera_ = camera; }
	void SetParticleSystem(ParticleSystem* particleSystem) { particleSystem_ = particleSystem; }
	void SetRailCamera(RailCamera* railCamera) { railCamera_ = railCamera; }
};

