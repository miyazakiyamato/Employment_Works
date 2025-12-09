#pragma once
#include "BaseWeapon.h"

class Input;
class TimeManager;
class ChargeGun : public BaseWeapon {
public:
	// コンストラクタ / デストラクタ
	ChargeGun() {};
	~ChargeGun() override {}

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

	void Charge() override;
private:
	Input* input_ = nullptr;
	TimeManager* timeManager_ = nullptr;
};

