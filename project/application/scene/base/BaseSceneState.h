#pragma once

class GameScene;

/// <summary>
/// ゲームシーンの状態基底クラス
/// </summary>
class BaseSceneState {
public:
	virtual ~BaseSceneState() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize(GameScene* gameScene);

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

protected:
	GameScene* gameScene_ = nullptr;
};
