#pragma once
#include "BaseScene.h"
#include "Sprite.h"
#include "Skydome.h"
#include "Ground.h"

class TitleScene : public BaseScene {
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
	//天球
	std::unique_ptr<Skydome> skydome_ = nullptr;
	//地面
	std::unique_ptr<Ground> ground_ = nullptr;
	//スプライト
	std::vector< std::unique_ptr<Sprite>> sprites_;
	//カメラ
	Camera* camera_ = nullptr;
};

