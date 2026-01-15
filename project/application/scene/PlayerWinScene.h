#pragma once
#include "BaseScene.h"
#include "PlayerWinUI.h"

/// <summary>
/// プレイヤー勝利シーン
/// </summary>
class PlayerWinScene : public BaseScene {
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
	//スプライト群
	std::unique_ptr<PlayerWinUI> playerWinUI_;

	//フェードの持続時間
	float duration_ = 1.0f;
	//経過時間カウンター
	float counter_ = 0.0f;
public:
	float GetDuration() const { return duration_; }
	float GetCounter() const { return counter_; }
};

