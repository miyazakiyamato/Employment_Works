#pragma once
#include "SmallDrone.h"

class BulletManager;
class TimeManager;
class SmallDroneStateShoot : public BaseEnemyState {
public:
	SmallDroneStateShoot(BaseEnemy* smallDrone);

	//初期化
	void Initialize()override;
	//更新
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

