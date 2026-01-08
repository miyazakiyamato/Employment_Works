#pragma once
#include <memory>
#include "BaseCharacter.h"
#include "TimedCall.h"
#include "BaseEnemyState.h"

class BulletManager;
class ParticleSystem;
class Player;
class BaseEnemy : public BaseCharacter {
public:
	struct Shake {
		Quaternion shakeQuaternion{};        // ターゲット回転（のけぞり先）
		Quaternion preShakeQuaternion{};     // 1フレーム前
		Quaternion startQuaternion{};        // 開始時の回転
		float kTime;
		float time;
		bool isShake = false;

		Vector3 move{};
		Vector3 preMove{};
	};

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize()override;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update()override;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

	// 衝突を検知したら呼び出されるコールバック関数
	void OnCollision([[maybe_unused]] Collider* other) override;

	//Updateのステートチェンジ
	void ChangeState(std::unique_ptr<BaseEnemyState> state);
	
	
protected:
	void Damage(int damage, const Vector3& hitDirection);
	void DamageKnockbackStart(const Vector3& hitDirection, float power, float duration);
	void ShakeStart(Quaternion shakeQuaternion, float kTime);
	void Shaking();
	// メンバ変数
	BulletManager* bulletManager_ = nullptr; // バレットマネージャー
	ParticleSystem* particleSystem_ = nullptr;
	Player* player_ = nullptr; // プレイヤーへの参照
	int hp_ = 3; // ヒットポイント
	bool isAlive_ = true; // 生存状態

	// 発射間隔
	float kFireInterval_ = 3.0f;

	// ステート
	std::unique_ptr<BaseEnemyState> state_;
	// シェイクデータ
	Shake shake_{};
public:
	/// <summary>
	/// パーティクルシステムの設定
	/// </summary>
	void SetParticleSystem(ParticleSystem* particleSystem) { particleSystem_ = particleSystem; }
	void SetBulletManager(BulletManager* bulletManager) { bulletManager_ = bulletManager; }
	void SetPlayer(Player* player) { player_ = player; }
	void SetPosition(const Vector3& position) { object3d_->SetTranslate(position); }
	void SetIsAlive(bool isAlive) { isAlive_ = isAlive; }
	void SetHP(int hp) { hp_ = hp; }

	bool GetIsAlive() const { return isAlive_; }
	int GetHP() const { return hp_; }
	float GetFireInterval() const { return kFireInterval_; }
	Player* GetPlayer() const { return player_; }
	BulletManager* GetBulletManager() const { return bulletManager_; }
};

