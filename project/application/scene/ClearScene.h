#pragma once
#include "BaseScene.h"
#include "Sprite.h"
#include "Skydome.h"
#include "Ground.h"
#include "Player.h"

/// <summary>
/// クリアシーン
/// </summary>
class ClearScene : public BaseScene {
public://メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 終了
	/// </summary>
	void Finalize() override;
	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;
private://メンバ変数
	//天球
	std::unique_ptr<Skydome> skydome_ = nullptr;
	//地面
	std::unique_ptr<Ground> ground_ = nullptr;
	//プレイヤー
	std::unique_ptr<Player> player_ = nullptr;
	//スプライト
	std::vector< std::unique_ptr<Sprite>> sprites_;

	float scaleCount_ = 0.0f;
};

