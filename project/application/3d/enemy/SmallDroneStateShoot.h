#pragma once
#include "SmallDrone.h"

class BulletManager;
class TimeManager;
/// <summary>
/// 小型ドローンの状態：射撃
/// </summary>
class SmallDroneStateShoot : public BaseEnemyState {
public:
	SmallDroneStateShoot(BaseEnemy* smallDrone);

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;
private:
	// 攻撃
	void Fire();
	void FireTimed();
	void FireCancel();
	// ポインタ
	SmallDrone* smallDrone_ = nullptr;
	Player* player_ = nullptr;
	BulletManager* bulletManager_ = nullptr;
	TimeManager* timeManager_ = nullptr;
	// 時限発射のリスト
	std::list<TimedCall*> timedCalls_;
};

