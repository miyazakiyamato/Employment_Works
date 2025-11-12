#pragma once
#include <vector>
#include <memory>
#include "Sprite.h"

class Player;
class HpUI{
public:
	//初期化
	void Initialize(Player* player);
	//終了
	void Finalize();
	//更新
	void Update();
	//描画
	void Draw();
	//imGui
	void ImGuiUpdate();
private:
	//プレイヤーデータ
	Player* player_ = nullptr;
	float maxHp_ = 5;
	float preHp_ = 5;
	float damageHp_ = 0;
	float easingDamageHp_ = 0.0f;
	float damegeHpTimer_ = 0.0f;
	float damegeHpLimitTime_ = 2.0f;
	Vector2 hpSize_ = { 300.0f,50.0f };
	//スプライト
	std::vector< std::unique_ptr<Sprite>> sprites_;
};

