#pragma once
#include <memory>
#include "BaseCharacter.h"
#include "TimedCall.h"

class BulletManager;
class ParticleSystem;
class Player;
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

	//発射間隔
	static const float inline kFireInterval = 1.0f;
	//
	void Fire();
	void FireTimed();
	void FireCancel();
private:
	BulletManager* bulletManager_ = nullptr; // バレットマネージャー
	ParticleSystem* particleSystem_ = nullptr;
	Player* player_ = nullptr; // プレイヤーへの参照
	int hp_ = 3; // ヒットポイント
	bool isAlive_ = true; // 生存状態
	//時限発射のリスト
	std::list<TimedCall*> timedCalls_;
public:
	/// <summary>
	/// パーティクルシステムの設定
	/// </summary>
	void SetParticleSystem(ParticleSystem* particleSystem) { particleSystem_ = particleSystem; }
	void SetBulletManager(BulletManager* bulletManager) { bulletManager_ = bulletManager; }
	void SetPlayer(Player* player) { player_ = player; }
	void SetPosition(const Vector3& position) { object3d_->SetTranslate(position); }
	void SetHP(int hp) { hp_ = hp; }

	bool GetIsAlive() const { return isAlive_; }
	int GetHP() const { return hp_; }
};

