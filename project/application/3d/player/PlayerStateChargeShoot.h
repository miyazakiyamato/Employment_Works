#pragma once
#include "BaseCharacterState.h"

class Input;
class TimeManager;
class Player;
class PlayerStateChargeShoot : public BaseCharacterState{
public:
	// コンストラクタ
	PlayerStateChargeShoot(BaseCharacter* character) : BaseCharacterState("PlayerRoot", character) {};
	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;
private:
	Input* input_ = nullptr;
	TimeManager* timeManager_ = nullptr;
	Player* player_ = nullptr;
};

