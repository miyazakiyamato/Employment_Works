#pragma once
#include "Input.h"

class SceneManager;
/// <summary>
/// シーン基底クラス
/// </summary>
class BaseScene{
public://メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize();
	/// <summary>
	/// 終了
	/// </summary>
	virtual void Finalize();
	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	virtual void Update();
	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;
private://メンバ変数

protected://継承
	SceneManager* sceneManager_ = nullptr;
	Input* input_ = nullptr;
	//シーン終了フラグ
	bool isFinished_ = false;
public://ゲッターセッター
	virtual void SetSceneManager(SceneManager* sceneManager) { sceneManager_ = sceneManager; }
	bool IsFinished() { return isFinished_; }
};

