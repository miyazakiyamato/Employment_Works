#pragma once
#include "BaseCharacterState.h"
#include "Vector3.h"

class Input;
class TimeManager;
class Player;
class PlayerStateLeave : public BaseCharacterState{
public:
	// コンストラクタ
	PlayerStateLeave(BaseCharacter* character) : BaseCharacterState("PlayerRoot", character) {};
	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;
private:
	Input* input_ = nullptr;
	TimeManager* timeManager_ = nullptr;
	Player* player_ = nullptr;

	Vector3 leaveVelocity_ = { 0.0f, 0.0f, 60.0f };
};

