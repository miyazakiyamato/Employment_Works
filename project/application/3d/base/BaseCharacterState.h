#pragma once
#include <string>

class BaseCharacter;
class BaseCharacterState {
public:
	// コンストラクタ / デストラクタ
	BaseCharacterState(const std::string& name, BaseCharacter* character) : name_(name), character_(character){};
	virtual ~BaseCharacterState() {}
		// --- メンバ関数 ---
	virtual void Initialize() = 0;
	virtual void Update() = 0;
protected:
	// 状態名
	std::string name_;
	// キャラクターへのポインタ
	BaseCharacter* character_ = nullptr;
};
