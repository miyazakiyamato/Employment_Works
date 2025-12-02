#pragma once
#include "BaseCharacterState.h"
#include "Vector3.h"

class Input;
class TimeManager;
class Player;
class PlayerStateClear : public BaseCharacterState{
public:
	// コンストラクタ
	PlayerStateClear(BaseCharacter* character) : BaseCharacterState("PlayerRoot", character) {};
	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;
private:
	Input* input_ = nullptr;
	TimeManager* timeManager_ = nullptr;
	Player* player_ = nullptr;

	// clear毎フレーム更新する角度（公転）
	float revolveAngle_ = 0.0f;
};

