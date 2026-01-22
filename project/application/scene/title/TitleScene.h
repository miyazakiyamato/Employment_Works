#pragma once
#include "BaseScene.h"
#include "BaseSceneState.h"
#include "Sprite.h"
#include "Skydome.h"
#include "Ground.h"
#include <vector>
#include "TitleUI.h"

/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene : public BaseScene {
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
	/// <summary>
	/// ステート変更
	/// </summary>
	void ChangeState(std::unique_ptr<BaseSceneState<TitleScene>> newState);

private://メンバ変数
	//天球
	std::unique_ptr<Skydome> skydome_ = nullptr;
	//地面
	std::unique_ptr<Ground> ground_ = nullptr;
	//カメラ
	Camera* camera_ = nullptr;

	//ステート
	std::unique_ptr<BaseSceneState<TitleScene>> state_;
public: // ゲッター
	Skydome* GetSkydome() { return skydome_.get(); }
	Ground* GetGround() { return ground_.get(); }

};
