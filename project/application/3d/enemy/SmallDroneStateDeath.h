#pragma once
#include "SmallDrone.h"

class BulletManager;
class TimeManager;
class SmallDroneStateDeath : public BaseEnemyState {
public:
	SmallDroneStateDeath(BaseEnemy* smallDrone);

	//初期化
	void Initialize()override;
	//更新
	void Update()override;
private:
	// ポインタ
	SmallDrone* smallDrone_ = nullptr;
	TimeManager* timeManager_ = nullptr;
	// 死亡用タイマー
	float kDethTimer = 2.0f;
	float dethTimer_ = 0.0f;
};

