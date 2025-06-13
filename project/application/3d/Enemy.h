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
public:
	/// <summary>
	/// パーティクルシステムの設定
	/// </summary>
	void SetParticleSystem(ParticleSystem* particleSystem) { particleSystem_ = particleSystem; }
};

