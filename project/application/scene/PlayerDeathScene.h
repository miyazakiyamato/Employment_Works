#pragma once
#include "BaseScene.h"
#include "Sprite.h"
#include <vector>

class PlayerDeathScene : public BaseScene {
public://メンバ関数
	//初期化
	void Initialize() override;
	//終了
	void Finalize() override;
	//毎フレーム更新
	void Update() override;
	//描画
	void Draw() override;
private://メンバ変数
	//スプライト群
	std::vector< std::unique_ptr<Sprite>> sprites_;

	//フェードの持続時間
	float duration_ = 1.0f;
	//経過時間カウンター
	float counter_ = 0.0f;
public:
	float GetDuration() const { return duration_; }
	float GetCounter() const { return counter_; }
};

