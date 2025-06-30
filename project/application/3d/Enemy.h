#pragma once
#include <memory>
#include "BaseCharacter.h"

class ParticleSystem;
class Enemy : public BaseCharacter {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	//衝突を検知したら呼び出されるコールバック関数
	void OnCollision([[maybe_unused]] Collider* other) override;
private:
	ParticleSystem* particleSystem_ = nullptr;

	int hp_ = 3; // ヒットポイント
	bool isAlive_ = true; // 生存状態
public:
	/// <summary>
	/// パーティクルシステムの設定
	/// </summary>
	void SetParticleSystem(ParticleSystem* particleSystem) { particleSystem_ = particleSystem; }
	void SetPosition(const Vector3& position) { object3d_->SetTranslate(position); }
	void SetHP(int hp) { hp_ = hp; }

	bool GetIsAlive() const { return isAlive_; }
	int GetHP() const { return hp_; }
};

